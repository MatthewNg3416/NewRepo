#include "stdafx.h"
#include <cstring>
#include "CRTDBG.h"
#include "windows.h"
#include "WinBase.h"
#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")
#pragma once

class CallStack
{
public:
	static const size_t kMaxStackDepth = 16U;

public:
	explicit CallStack()
	{
		count_ = 0, hash_ = 0;
		for (int i = 0; i < kMaxStackDepth; i++)
			addresses_[i] = NULL;
	}

	void* operator[] (const size_t index) const { return addresses_[index]; }

	ULONG GetHash() const { return hash_; }
	size_t GetCount() const { return count_; }

protected:
	void* addresses_[kMaxStackDepth];
	ULONG hash_;
	size_t count_;
};
struct SymbolBuffer : public SYMBOL_INFO
{
	SymbolBuffer()
	{
		// SYMBOL_INFO 구조체에 필요한 정보를 setting
		MaxNameLen = static_cast<ULONG>(kBufferSize);
		SizeOfStruct = sizeof(SYMBOL_INFO);
	}

	static const size_t kBufferSize = 256U;
	char buffer_[kBufferSize];
};



class MStackTrace : public CallStack
{
	const static size_t kStrBufferSize = 1024U;

private:
	BOOL bInitalize;
	HANDLE handle_;
	CString strComment;

	CString MStackTrace::GetSymbolString(void* address) const
	{
		if (address == nullptr)
			return "";

		SymInitialize(handle_, nullptr, TRUE);
		SymSetOptions(SYMOPT_LOAD_LINES);
		
		DWORD displacement = 0;
		DWORD addr = reinterpret_cast<DWORD>(address);
		SymbolBuffer symbol;
		char buffer[kStrBufferSize] = { 0, };

		// 주소에 대응하는 심볼의 파일이름 + line을 저장할 객체       
		IMAGEHLP_LINE line;
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

		// SymbolBuffer 객체에 address에 대응하는 심볼 이름(함수명) 저장
		SymFromAddr(handle_, addr, 0, &symbol);


		BOOL ret = SymGetLineFromAddr(handle_, addr, &displacement, &line);
		if (ret) 
		{
			sprintf_s(buffer, kStrBufferSize, "%s(%d) : %s",
				line.FileName, line.LineNumber, symbol.Name);
		}
		else {
			sprintf_s(buffer, kStrBufferSize, "No line info : %s", symbol.Name);
		}
		return CString(buffer);
	};
public:
	MStackTrace() : CallStack()
	{
		bInitalize = FALSE;
		strComment = _T("");
	}
	MStackTrace(CString Comment) : CallStack()
	{
		bInitalize = FALSE;
		strComment = Comment;
	}

	~MStackTrace()
	{
	}

	MStackTrace* ReadCallStack()
	{
		bInitalize = TRUE;
		count_ = CaptureStackBackTrace(0, kMaxStackDepth, addresses_, &hash_);
		handle_ = GetCurrentProcess();

		return this;
	}

	CString MStackTrace::GetString(int iIndex)
	{
		if (bInitalize == FALSE || iIndex < 0 || iIndex > (int)count_)
			return "";
		return GetSymbolString(addresses_[iIndex]);
	}
};


