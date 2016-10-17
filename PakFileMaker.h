#pragma once
#include "PakFile.h"

namespace pk
{
	class PakFileMaker // creates a PakFile file, on fixed storage
	{
	public:
		// generates PakFile with the file name, version number, and std::string file names begin through end
		template <typename InputIterator>
		void operator()(const std::string &fileName,boost::uint32_t version,InputIterator &begin,InputIterator &end)
		{
			// create temporary header
			Header header;
			header.version=version;
			// get number of entries
			header.nEntries=std::distance(begin,end);
			// copy paths and sort by hash
			std::vector<EntryPID> pathList;
			pathList.reserve(header.nEntries);
			std::copy(begin,end,std::back_inserter(pathList));
			std::sort(pathList.begin(),pathList.end());
			typedef std::vector<EntryPID>::iterator plIt_t; // path list iterator type
			// open file stream
			std::ofstream fs(fileName.c_str(),std::ios::binary|std::ios::out|std::ios::trunc);
			// write temporary header
			iol::Write(fs,header);
			// write FAT table
			plIt_t i = pathList.begin(); // current path list iterator
			std::vector<boost::uint32_t> posList; // list of FAT entry positions
			posList.reserve(header.nEntries);
			while (i!=pathList.end())
			{
				// copy current position of FAT table entry
				posList.push_back(fs.tellp());
				// write entry file name length in bytes
				iol::Write(fs,i->path.length());
				// write entry file name
				iol::Write(fs,i->path.begin(),i->path.end());
				// write temporary entry location
				iol::Write(fs,boost::uint32_t(0));
				// write temporary entry size
				iol::Write(fs,boost::uint32_t(0));
				// increment path list iterator
				++i;
			}
			// write raw file data for each entry
			std::ifstream eFS; // entry file stream
			i = pathList.begin();// reset path list iterator
			std::vector<boost::uint32_t>::iterator posListI = posList.begin(); // FAT entry positions iterator
			header.FATEndPos = fs.tellp(); // get end p

			while (i!=pathList.end())
			{
				// open entry file
				eFS.open(i->path.c_str(),std::ios::binary|std::ios::in);
					// store current PakFile position
					boost::uint32_t pos=fs.tellp();
					// rewind PakFile to FAT table entry for current file and skip entry file name
					fs.seekp(*posListI+sizeof(boost::uint32_t)+i->path.length());
					// write entry location
					iol::Write(fs,pos);
					// write entry size
					eFS.seekg(0,std::ios::end);
					boost::uint32_t sz=static_cast<boost::uint32_t>(eFS.tellg()); // get size of entry file
					iol::Write(fs,sz);
					eFS.seekg(0,std::ios::beg);
					// skip back to original PakFile position
					fs.seekp(pos);
					// copy file into PakFile
					fs<<eFS.rdbuf();
					// close entry file
				eFS.close();
				// increment iterators
				++posListI;
				++i;
			}
			header.fileSize=fs.tellp(); // get file size
			// rewind to header
			fs.seekp(0);
			// write completed header
			iol::Write(fs,header);
		}
	private:
		// path and id pair for entries, used to sort entries
		class EntryPID
		{
		public:
			EntryPID() : id(0) {}
			EntryPID(const EntryPID &e) : path(e.path), id(e.id) {};
			EntryPID(std::string path_) : path(path_),id(Entry::Hash(path_)) {};
			EntryPID &operator=(const EntryPID &e) { id=e.id;path=e.path;return *this; };
			EntryPID &operator=(const std::string &path_) { id=Entry::Hash(path_);path=path_;return *this; };
			bool operator==(const EntryPID &e) const { return id==e.id; };
			bool operator<(const EntryPID &e) const { return id<e.id; };
			bool operator>(const EntryPID &e) const { return id>e.id; };

			std::string path; // entry path
			Entry::keyType_t id; // entry id
		};
	};
};
