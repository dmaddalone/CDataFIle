//
// CDataFile Class Implementation
//
// The purpose of this class is to provide a simple, full featured means to
// store persistent data to a text file.  It uses a simple key/value paradigm
// to achieve this.  The class can read/write to standard Windows .ini files,
// and yet does not rely on any windows specific calls.  It should work as
// well in a linux environment (with some minor adjustments) as it does in
// a Windows one.
//
// Written July, 2002 by Gary McNickle <gary#sunstorm.net>
// If you use this class in your application, credit would be appreciated.
//
// Minor updates October 2014 by Dom Maddalone <dominick.maddalone#gmail.com>
//      Focused on making compatible with C++11
//      Removed typedef std::string t_Str
//      Added CheckSectionName public method
//

#ifndef __CDATAFILE_H__
#define __CDATAFILE_H__

#include <vector>
//#include <fstream.h>
#include <fstream>
#include <string>

#include <locale>

// Globally defined structures, defines, & types
//////////////////////////////////////////////////////////////////////////////////

// AUTOCREATE_SECTIONS
// When set, this define will cause SetValue() to create a new section, if
// the requested section does not allready exist.
#define AUTOCREATE_SECTIONS     (1L<<1)

// AUOTCREATE_KEYS
// When set, this define causes SetValue() to create a new key, if the
// requested key does not allready exist.
#define AUTOCREATE_KEYS         (1L<<2)

// MAX_BUFFER_LEN
// Used simply as a max size of some internal buffers. Determines the maximum
// length of a line that will be read from or written to the file or the
// report output.
#define MAX_BUFFER_LEN				512


// eDebugLevel
// Used by our Report function to classify levels of reporting and severity
// of report.
enum e_DebugLevel
{
	// detailed programmatic informational messages used as an aid in
	// troubleshooting problems by programmers
	E_DEBUG = 0,
	// brief informative messages to use as an aid in troubleshooting
	// problems by production support and programmers
	E_INFO,
	// messages intended to notify help desk, production support and
	// programmers of possible issues with respect to the running application
	E_WARN,
	// messages that detail a programmatic error, these are typically
	// messages intended for help desk, production support, programmers and
	// occasionally users
	E_ERROR,
	// severe messages that are programmatic violations that will usually
	// result in application failure. These messages are intended for help
	// desk, production support, programmers and possibly users
	E_FATAL,
	// notice that all processing should be stopped immediately after the
	// log is written.
	E_CRITICAL
};


//typedef std::string t_Str;

// CommentIndicators
// This constant contains the characters that we check for to determine if a
// line is a comment or not. Note that the first character in this constant is
// the one used when writing comments to disk (if the comment does not allready
// contain an indicator)
const std::string CommentIndicators = std::string(";#");

// EqualIndicators
// This constant contains the characters that we check against to determine if
// a line contains an assignment ( key = value )
// Note that changing these from their defaults ("=:") WILL affect the
// ability of CDataFile to read/write to .ini files.  Also, note that the
// first character in this constant is the one that is used when writing the
// values to the file. (EqualIndicators[0])
const std::string EqualIndicators   = std::string("=:");

// WhiteSpace
// This constant contains the characters that the Trim() function removes from
// the head and tail of strings.
const std::string WhiteSpace = std::string(" \t\n\r");

// st_key
// This structure stores the definition of a key. A key is a named identifier
// that is associated with a value. It may or may not have a comment.  All comments
// must PRECEDE the key on the line in the config file.
typedef struct st_key
{
	std::string		szKey;
	std::string		szValue;
	std::string		szComment;

	st_key()
	{
		szKey = std::string("");
		szValue = std::string("");
		szComment = std::string("");
	}

} t_Key;

typedef std::vector<t_Key> KeyList;
typedef KeyList::iterator KeyItor;

// st_section
// This structure stores the definition of a section. A section contains any number
// of keys (see st_keys), and may or may not have a comment. Like keys, all
// comments must precede the section.
typedef struct st_section
{
	std::string		szName;
	std::string		szComment;
	KeyList		    Keys;

	st_section()
	{
		szName = std::string("");
		szComment = std::string("");
		Keys.clear();
	}

} t_Section;

typedef std::vector<t_Section> SectionList;
typedef SectionList::iterator SectionItor;



/// General Purpose Utility Functions ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//void	Report(e_DebugLevel DebugLevel, char *fmt, ...);
void	Report(e_DebugLevel DebugLevel, const char *fmt, ...);
std::string	GetNextWord(std::string& CommandLine);
int		CompareNoCase(std::string str1, std::string str2);
void	Trim(std::string& szStr);
//int		WriteLn(fstream& stream, char* fmt, ...);
int		WriteLn(std::fstream& stream, const char* fmt, ...);


/// Class Definitions ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


// CDataFile
class CDataFile
{
// Methods
public:
				// Constructors & Destructors
				/////////////////////////////////////////////////////////////////
				CDataFile();
				CDataFile(std::string szFileName);
	virtual		~CDataFile();

				// File handling methods
				/////////////////////////////////////////////////////////////////
	bool		Load(std::string szFileName);
	bool		Save();

				// Data handling methods
				/////////////////////////////////////////////////////////////////

				// GetValue: Our default access method. Returns the raw t_Str value
				// Note that this returns keys specific to the given section only.
	std::string		GetValue(std::string szKey, std::string szSection = std::string(""));
				// GetString: Returns the value as a t_Str
	std::string		GetString(std::string szKey, std::string szSection = std::string(""));
				// GetFloat: Return the value as a float
	float		GetFloat(std::string szKey, std::string szSection = std::string(""));
				// GetInt: Return the value as an int
	int			GetInt(std::string szKey, std::string szSection = std::string(""));
				// GetBool: Return the value as a bool
	bool		GetBool(std::string szKey, std::string szSection = std::string(""));

                // CheckSectionName: Return true if section name exists; false if not (Maddalone)
	bool        CheckSectionName(std::string szSectionName);

				// SetValue: Sets the value of a given key. Will create the
				// key if it is not found and AUTOCREATE_KEYS is active.
	bool		SetValue(std::string szKey, std::string szValue,
						 std::string szComment = std::string(""), std::string szSection = std::string(""));

				// SetFloat: Sets the value of a given key. Will create the
				// key if it is not found and AUTOCREATE_KEYS is active.
	bool		SetFloat(std::string szKey, float fValue,
						 std::string szComment = std::string(""), std::string szSection = std::string(""));

				// SetInt: Sets the value of a given key. Will create the
				// key if it is not found and AUTOCREATE_KEYS is active.
	bool		SetInt(std::string szKey, int nValue,
						 std::string szComment = std::string(""), std::string szSection = std::string(""));

				// SetBool: Sets the value of a given key. Will create the
				// key if it is not found and AUTOCREATE_KEYS is active.
	bool		SetBool(std::string szKey, bool bValue,
						 std::string szComment = std::string(""), std::string szSection = std::string(""));

				// Sets the comment for a given key.
	bool		SetKeyComment(std::string szKey, std::string szComment, std::string szSection = std::string(""));

				// Sets the comment for a given section
	bool		SetSectionComment(std::string szSection, std::string szComment);

				// DeleteKey: Deletes a given key from a specific section
	bool		DeleteKey(std::string szKey, std::string szFromSection = std::string(""));

				// DeleteSection: Deletes a given section.
	bool		DeleteSection(std::string szSection);

				// Key/Section handling methods
				/////////////////////////////////////////////////////////////////

				// CreateKey: Creates a new key in the requested section. The
	            // Section will be created if it does not exist and the
				// AUTOCREATE_SECTIONS bit is set.
	bool		CreateKey(std::string szKey, std::string szValue,
		                  std::string szComment = std::string(""), std::string szSection = std::string(""));
				// CreateSection: Creates the new section if it does not allready
				// exist. Section is created with no keys.
	bool		CreateSection(std::string szSection, std::string szComment = std::string(""));
				// CreateSection: Creates the new section if it does not allready
				// exist, and copies the keys passed into it into the new section.
	bool		CreateSection(std::string szSection, std::string szComment, KeyList Keys);

				// Utility Methods
				/////////////////////////////////////////////////////////////////
				// SectionCount: Returns the number of valid sections in the database.
	int			SectionCount();
				// KeyCount: Returns the total number of keys, across all sections.
	int			KeyCount();
				// Clear: Initializes the member variables to their default states
	void		Clear();
                // Maddalone:
                // ClearDirty: clear m_bDirty flag - so we don't try to save the file
    void        ClearDirty();
				// SetFileName: For use when creating the object by hand
				// initializes the file name so that it can be later saved.
	void		SetFileName(std::string szFileName);
				// CommentStr
				// Parses a string into a proper comment token/comment.
	std::string		CommentStr(std::string szComment);


protected:
				// Note: I've tried to insulate the end user from the internal
				// data structures as much as possible. This is by design. Doing
				// so has caused some performance issues (multiple calls to a
				// GetSection() function that would otherwise not be necessary,etc).
				// But, I believe that doing so will provide a safer, more stable
				// environment. You'll notice that nothing returns a reference,
				// to modify the data values, you have to call member functions.
				// think carefully before changing this.

				// GetKey: Returns the requested key (if found) from the requested
				// Section. Returns NULL otherwise.
	t_Key*		GetKey(std::string szKey, std::string szSection);
				// GetSection: Returns the requested section (if found), NULL otherwise.
	t_Section*	GetSection(std::string szSection);


// Data
public:
	long		m_Flags;		// Our settings flags.

protected:
	SectionList	m_Sections;		// Our list of sections
	std::string		m_szFileName;	// The filename to write to
	bool		m_bDirty;		// Tracks whether or not data has changed.
};


#endif
