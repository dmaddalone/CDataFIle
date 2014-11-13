/// DataFileTest.cpp ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//
// This file is a simple test of the features of the CDataFile object. The
// purpose is to both test the methods of the class, and provide examples
// on how to use those methods.
////////////////////////////////////////////////////////////////////////////

//#include <stdio.h>
#include <cstdio>
//#include <float.h>	// needed for the FLT_MIN define
#include <cfloat>	// needed for the FLT_MIN define

#include <climits>

#include "CDataFile.h"

void doSomething()
{
	/// Section One ////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// In this section, we are going to create a CDataFile object by opening
	// an allready existing file.  If the file is sucessfully opened, the
	// CDataFile will be populated with it's key/value pair contents.
	////////////////////////////////////////////////////////////////////////////

	// Create the object, passing in the file name. The file will be loaded
	// (if it can be) and the objects keys set to the contents of the file.
	// The file is then closed.
	CDataFile ExistingDF("CrapSim.ini");

	Report(E_INFO, "[doSomething] The file <CrapSim.ini> contains %d sections, & %d keys.",
				   ExistingDF.SectionCount(), ExistingDF.KeyCount());

	// Querry the CDataFile for the values of some keys ////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Keys that are not found in the object will have the following values;
	//
	//   String based keys: t_Str("")
	//   Int based keys   : INT_MIN
	//   Float based keys : FLT_MIN
	////////////////////////////////////////////////////////////////////////////
	std::string szAuthor  = std::string("");
	float fValue	= 0.0f;
	int	  nValue	= 0;

	std::string szTableType = ExistingDF.GetString("Type", "Table");
	if ( szTableType.size() == 0 )
		Report(E_INFO, "[doSomething] Key 'Type' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'Type' contains the value '%s'",
						szTableType.c_str());

    int nTableMinWager = ExistingDF.GetInt("MinimumWager", "Table");
	if ( nTableMinWager == INT_MIN)
		Report(E_INFO, "[doSomething] Key 'MinimumWager' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'MinimumWager' contains the value '%d'",
						nTableMinWager);

    int nTableMaxWager = ExistingDF.GetInt("MaximumWager", "Table");
	if ( nTableMaxWager == INT_MIN)
		Report(E_INFO, "[doSomething] Key 'MaximumWager' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'MaximumWager' contains the value '%d'",
						nTableMaxWager);

	szAuthor = ExistingDF.GetString("author");
	if ( szAuthor.size() == 0 )
		Report(E_INFO, "[doSomething] Key 'author' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'author' contains the value '%s'",
						szAuthor.c_str());

	fValue  = ExistingDF.GetFloat("main_key_float", "Main");
	if ( fValue == FLT_MIN )
		Report(E_INFO, "[doSomething] Key 'main_key_float' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'main_key_float' contains the value '%f'",
						fValue);

	nValue  = ExistingDF.GetInt("non existent key", "Main");

	if ( nValue == INT_MIN )
		Report(E_INFO, "[doSomething] Key 'non existent key' was not found.");
	else
		Report(E_INFO, "[doSomething] Key 'non existent key' contains the value '%d'",
		               nValue);



	// Test setting some key values ////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	// Turn off the AUTOCREATE_KEYS behavior.
	ExistingDF.m_Flags &= ~AUTOCREATE_KEYS;

	// With AUTOCREATE_KEYS off, SetValue will fail if it cannot find
	// the requested key. If it does find it, it will set the keys new
	// value.
	if ( ExistingDF.SetValue("main_key_float", "9.876543210", "", "Main") )
		Report(E_INFO, "[doSomething] Key 'main_key_float' value changed.");
	else
		Report(E_INFO, "[doSomething] Key 'main_key_float' not found. Value not set.");

	// Use SetValue to create a new key

	// Set the AUTOCREATE_KEYS flag.
	ExistingDF.m_Flags |= AUTOCREATE_KEYS;

	if ( ExistingDF.SetValue("new_key", "new_key_value", "Procedurally generated key", "Main") )
		Report(E_INFO, "[doSomething] Key 'new_key' successfully added.");
	else
		Report(E_INFO, "[doSomething] Key 'new_key' was not added.");


	/// Section Two ////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// In this section, we demonstrate how to create a new CDataFile object
	// and populate it with some keys. We'll also show how to use some of the
	// other methods.
	////////////////////////////////////////////////////////////////////////////

	// Test creating a new data file procedurally.
	CDataFile NewDF;

	// At this point, NewDF contains one section, the default section named "".

	// Add a couple of new keys to the default section.
	NewDF.CreateKey("Letters", "abcdefghijklmnopqrstuvwxyz");
	NewDF.CreateKey("Numbers", "0123456789");

	// Create a new section
	// The first value is the section name, the second value is the comment.
	NewDF.CreateSection("ApplicationSettings",
						"; This section contains all of the generic application settings.");

	// Add some keys to our new section. The arguments are;
	// 1) the key name
	// 2) The key value
	// 3) The comment for the key (empty comments are not written to disk)
	// 4) The section to put this value in. (Section must exist)
	NewDF.CreateKey("install_path",
				   "c:\\projects\\cdatafile\\",
		           "; The path where this project has been installed to.",
				   "ApplicationSettings");

	NewDF.CreateKey("version",
				   "01",
				   "; The current version",
				   "ApplicationSettings");

	// A note about default values /////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Several of these functions use default empty values (t_Str("")) for
	// the comment and section fields, this can bite you if your not careful.
	// For example;
	//
	// NewDF.SetValue("date compiled",
	//			   "July 23rd, 2002",
	//			   "Settings");
	//
	// While it looks like your passing the Section name in (as Settings)
	// in fact, this is the comment field. Doing this will cause unexpected
	// results to show up in your file. (It will look like you have a section
	// named "Settings", but it will not have the [] brackets around
	// it).  This is easy to spot once you know what your looking for, but
	// it's definately something to watch out for.
	////////////////////////////////////////////////////////////////////////////


	// Add a key with SetValue, that contains spaces.
	NewDF.SetValue("date compiled",
				   "July 23rd, 2002",
				   "",
				   "ApplicationSettings");

	// Now, if we had read this data file from disk, then the file name
	// would allready be stored, but since we created it procedurally, we
	// have to provide the filename.
	NewDF.SetFileName("new.ini");


	// A note about my use of Save() ///////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// When testing this, what I like to do is open the 'new' and 'test'.ini
	// files up in a text editor and step through this program line by line.
	// This way, after each save, I can reload the changes in the editor and
	// see whether or not the output is what I expected.
	////////////////////////////////////////////////////////////////////////////

	// Ok, test some of our access methods...

	NewDF.SetBool("is_bool", true);
	NewDF.SetInt("is_int", 100);
	NewDF.SetFloat("is_float", 3.21f);

	NewDF.SetSectionComment("ApplicationSettings", "Application Settings Comment");
	NewDF.SetKeyComment("install_path", "; Location installed to.", "ApplicationSettings");
	NewDF.Save();
	NewDF.DeleteKey("version", "ApplicationSettings");
	NewDF.Save();


	// Uncomment this next line to test Section deletion.
	//	NewDF.DeleteSection("ApplicationSettings");



	// A note about comments ///////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Comments are generally your responsibility.  While the code does append
	// a comment indicator (as defined in CDataFile.h, it will append the first
	// indicator in the constant) it makes no attempt to validate multiline
	// comments.
	////////////////////////////////////////////////////////////////////////////

	Report(E_INFO, "[doSomething] The file <new.ini> contains %d sections, & %d keys.",
				   NewDF.SectionCount(), NewDF.KeyCount());

	// If we did not explicitly call Save here, then the destructor would save the
	//  file since the data has been modified.  But we'll call it anyway.
	NewDF.Save();


	/// Section Three //////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	// Test our ability to read/write to a standard Windows .ini file.
	////////////////////////////////////////////////////////////////////////////
	CDataFile WinDF("win.ini");

	Report(E_INFO, "[doSomething] The file <win.ini> contains %d sections, & %d keys.",
				   WinDF.SectionCount(), WinDF.KeyCount());

	WinDF.Save();
}

int main(int argc, char* argv[])
{
	doSomething();

	return  0;
}
