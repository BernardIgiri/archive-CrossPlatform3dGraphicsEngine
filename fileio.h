#pragma once

namespace io
{
	namespace fs = boost::filesystem;

	// fills dest with all files in dir that return true for predicate
	template <typename OutputIterator,typename Predicate>
	void FindFiles(const fs::path &dir,Predicate predicate,OutputIterator &dest, bool searchSubDirs=false)
	{
		if (!fs::exists(dir))
			return;
		for (fs::directory_iterator i(dir);i!=fs::directory_iterator();i++)
		{
			if (!fs::is_directory(*i))
			{
				if (predicate(i->leaf()))
					*dest++ =i->leaf();
			}
			else if (searchSubDirs)
				FindFilesDeep(*i,predicate,dest,std::string(i->leaf())+std::string("/"));
		}
	};
	// fills dest with all files in dir that return true for predicate
	// and searches all sub directories while appending curDir string to file name
	template <typename OutputIterator,typename Predicate>
	void FindFilesDeep(const fs::path &dir,Predicate predicate, OutputIterator &dest,std::string &curDir)
	{
		if (!fs::exists(dir))
			return;
		for (fs::directory_iterator i(dir);i!=fs::directory_iterator();i++)
		{
			if (!fs::is_directory(*i))
			{
				if (predicate(i->leaf()))
					*dest++ =curDir+i->leaf();
			}
			else
				FindFilesDeep(*i,predicate,dest,curDir+std::string(i->leaf())+std::string("/"));
		}
	};

	// Helper Predicates

	// Returns true for all files
	class MatchAll
	{
	public:
		MatchAll() {};
		~MatchAll() {};
		bool operator()(const std::string &fname) const { return true; };
	};

	// Returns true for all files with extension ext
	class MatchExt
	{
	public:
		MatchExt() {};
		MatchExt(const char *ext) { extension = ext; };
		MatchExt(const std::string &ext) { extension=ext; };
		~MatchExt() {};
		bool operator()(const std::string &fname) const;
		std::string extension;
	};

	
	// Returns true for all files with extensions begin_ through end_ where begin and end point to std::string
	template <typename InputIterator>
	class MatchExts
	{
	public:
		MatchExts() {};
		MatchExts(InputIterator &begin_,InputIterator &end_) { begin=begin_;end=end_; };
		~MatchExts() {};
		bool operator()(const std::string &fname) const
		{
			InputIterator i=begin;
			while (i!=end)
				if (IsMatch(fname,*i++))
					return true;
			return false;
		};
		bool IsMatch(const std::string &fname,const std::string &ext) const
		{
			if (fname.length()>ext.length())
				return (fname.substr(fname.length()-ext.length(),ext.length())==ext);
			return false;
		};

		InputIterator begin;
		InputIterator end;
	};
};