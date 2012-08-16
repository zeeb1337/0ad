/* Copyright (C) 2012 Wildfire Games.
 * This file is part of 0 A.D.
 *
 * 0 A.D. is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * 0 A.D. is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with 0 A.D.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_SOUNDDATA_H
#define INCLUDED_SOUNDDATA_H

#include "lib/external_libraries/openal.h"
#include "lib/file/vfs/vfs_path.h"
#include "lib/os_path.h"

#include <string>
#include <map>

class CSoundData;
typedef std::map<std::wstring, CSoundData*> DataMap;


class CSoundData
{
public:
	static CSoundData* SoundDataFromFile(const VfsPath& itemPath);
	static CSoundData* SoundDataFromOgg(const VfsPath& itemPath);

	static void ReleaseSoundData(CSoundData* theData);

	CSoundData();
	CSoundData(ALuint dataSource);
	virtual ~CSoundData();
	
	CSoundData* IncrementCount();
	bool DecrementCount();
	void InitProperties();
	virtual bool IsOneShot();

	
	virtual ALuint GetBuffer();
	virtual ALsizei GetBufferCount();
	CStrW GetFileName();
	virtual ALuint* GetBufferPtr();

protected:
	static DataMap* sSoundData;

	ALuint m_ALBuffer;
	int m_RetentionCount;
	CStrW m_FileName;

};

#endif // INCLUDED_SOUNDDATA_H
