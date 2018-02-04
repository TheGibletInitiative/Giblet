//	Copyright (c) 2018 Chet Simpson & The Giblet Initiative
//	
//	This file is subject to the terms and conditions defined in
//	file 'LICENSE.MD', which is part of this source code package.
//
#include "stdafx.h"
#include "cpuIA32.h"
#include "cpu_Win32MemoryBlock.h"
#include <list>

#ifdef _DEBUG
#define DEBUG_OUT debug_dbgprintf
#else
#define DEBUG_OUT(...)
#endif


namespace Giblet { namespace CodeHook { namespace x86
{

	static void debug_dbgprint(const char *format, va_list vl)
	{
		char			dbgbuf1[2048];
		char			dbgbuf2[2048];

		wvsprintfA(dbgbuf1, format, vl);
		wsprintfA(dbgbuf2, "YTPH: %s", dbgbuf1);
		OutputDebugStringA(dbgbuf2);
	}


	void debug_dbgprintf(const char *fmt, ...)
	{
		va_list marker;

		va_start(marker, fmt);

		// Send to a debug monitor
		debug_dbgprint(fmt, marker);
	}


	void debug_dbgprintf(int, const char *fmt, ...)
	{
		va_list marker;

		va_start(marker, fmt);

		// Send to a debug monitor
		debug_dbgprint(fmt, marker);
	}




#ifdef INCLUDE_TESTING
	void DecodeStream(const void *ptr, size_t maxSize)
	{
		OpcodeIterator		opIter(static_cast<const uint8_t*>(ptr));
		uint64_t			address;
		uint64_t			totalBytes;

		address = reinterpret_cast<uint64_t>(ptr);
		totalBytes = 0;
		while(true)
		{
			Instruction	opcode;
			bool		result;
			uint64_t	opcodeSize;

			result = opcode.Decode(opIter);
			if(false == result)
			{
				DEBUG_OUT("Decode Failed at %p (%02X %02X)\n", opIter.c_data(), opIter[0], opIter[1]);
				break;
			}

			opcodeSize = opcode.getSize();
			if(0 != maxSize && (totalBytes + opcodeSize) > maxSize)
			{
				break;
			}

			opcode.Relocate(address);
			opcode.Dump();
			address += opcodeSize;
			totalBytes += opcodeSize;
		}
	}
#endif




	size_t RelocateCode(
		uint8_t *dstPtr,
		const void *srcPtr,
		uint64_t minCodeBytes,
		uint64_t maxBufferSize)
	{
		OpcodeIterator		opIter(static_cast<const uint8_t*>(srcPtr));
		uint64_t				address;
		size_t				totalBytes;

		address = reinterpret_cast<uint64_t>(dstPtr);
		totalBytes = 0;

		//	Relocate
		while((totalBytes < minCodeBytes))
		{
			Instruction		opcode;
			bool					result;
			size_t					opcodeSize;

			//	Process the opcode
			result = opcode.Decode(opIter);
			if(false == result)
			{
				DEBUG_OUT("Relocate Failed at %p (%02X %02X)\n", opIter.c_data(), opIter[0], opIter[1]);
				totalBytes = 0;
				break;
			}

			opcodeSize = static_cast<size_t>(opcode.getSize());

			//	Check to see if it will fit
			if((totalBytes + opcodeSize) >= maxBufferSize)
			{
				DEBUG_OUT("Relocate exceeded buffer at %p (%02X %02X)\n", opIter.c_data(), opIter[0], opIter[1]);
				totalBytes = 0;
				break;
			}

			//	Relocate the opcode and generate the new code
			opcode.Relocate(address);
			dstPtr = opcode.Emit(dstPtr);

			//	Adjust counts and offsets
			address += opcodeSize;
			totalBytes += opcodeSize;
		}

		return totalBytes;
	}


	void HookError(const char *module, const char *name, HOOK_ERROR status)
	{
		const char *errorMsg;
		switch(status)
		{
		case HOOK_NOMODULE:
			errorMsg = "Unable to find DLL";
			break;

		case HOOK_NOFUNC:
			errorMsg = "The DLL does not contain the requested function";
			break;

		case HOOK_NORELOCATE:
			errorMsg = "Unable to install hook stub";
			break;

		case HOOK_NOREDIRECT:
			errorMsg = "Unable to install hook proxy";
			break;

		case HOOK_COREFAILED:
			errorMsg = "An unexpected failure expected in the core integration system.";
			break;

		case HOOK_OUTOFMEMORY:
			errorMsg = "Unable to allocate memory for integration bridge";
			break;

		default:
			errorMsg = "Unknown.";
		}

		char str[4096];

		sprintf(str, 
			"Encountered error #%d while initializing the integration module.\n"
			"Hook %s failed on module %s\n"
			"\n\nThe following status message was returned:\n\n     %s",
			status,
			name,
			module,
			errorMsg);

		MessageBoxA(NULL, str, "Integration Failed!", MB_OK | MB_ICONERROR);
		ExitProcess(1);
	}


	HOOK_ERROR InstallHook(
		const char *const	moduleName,
		const char *const	functionName,
		const void *const	proxyFunction,
		void **const		thunkBufferOut)
	{
		HMODULE				module;
		void				*procAddress;

		//	Get the module containing the function
		module = GetModuleHandleA(moduleName);
		if(NULL == module)
		{
			return HOOK_NOMODULE;
		}

		//	Get the address of the function we are hooking
		procAddress = GetProcAddress(module, functionName);
		if(NULL == procAddress)
		{
			return HOOK_NOFUNC;
		}


		return InstallHook(moduleName, functionName, procAddress, proxyFunction, thunkBufferOut);
	}


	HOOK_ERROR InstallHook(
		const char *const	moduleName,
		const char *const	functionName,
		const void *const	procAddress,
		const void *const	proxyFunction,
		void **const		thunkFunctionOut)
	{
		static const size_t	thunkBufferSize = THUNK_BUFFER_SIZE;

		void	*codeBuffer;
		//void	*thunkJumpPtr;
		//void	*thunkFunctionPtr;
		uint8_t	*ptr;


		//	Allocate the code buffer
		codeBuffer = VirtualAlloc(NULL, thunkBufferSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		if(NULL == codeBuffer)
		{
			HookError(moduleName, functionName, HOOK_OUTOFMEMORY);
			return HOOK_OUTOFMEMORY;
		}

		memset(codeBuffer, 0, thunkBufferSize);


		//	Set up the pointers
		ptr = static_cast<uint8_t*>(codeBuffer);

		void	*const thunkJumpPtr = ptr;
		ptr += sizeof(void*) * 4;
		void	*const thunkFunctionPtr = ptr;



		HOOK_ERROR	status;
		Assembler			procAssembler;
		size_t				thunkBufferUsed;

		status = HOOK_SUCCESS;
		thunkBufferUsed = 0;
		procAssembler = procAddress;


		//	Copy the code into the thunk buffer and append a
		//	call to the original code
		if(HOOK_SUCCESS == status)
		{
			bool		result;
			MemoryBlock	thunkLock(codeBuffer);

			result = thunkLock.Lock();
			if(false == result)
			{
				status = HOOK_COREFAILED;
			}
			else
			{
				//	Set the redirect jump address
				(static_cast<const void**>(thunkJumpPtr))[0] = proxyFunction;
				(static_cast<const void**>(thunkJumpPtr))[1] = thunkFunctionPtr;

				// Try relocating the code
				thunkBufferUsed = RelocateCode(
					static_cast<uint8_t*>(thunkFunctionPtr),
					procAddress,
					6,
					thunkBufferSize);
				if(0 == thunkBufferUsed)
				{
					status = HOOK_NORELOCATE;
				}

				else
				{
					//	Code has been relocated!
					Assembler	xasm(thunkFunctionPtr, thunkBufferUsed);

					xasm.Jump(reinterpret_cast<const uint8_t*>(procAddress) + thunkBufferUsed);
					thunkBufferUsed += 5;
				}
			}
		}

		//	Rewrite the code to jump to our thunk
		if(HOOK_SUCCESS == status)
		{
			bool		result;
			MemoryBlock	procLock(procAddress);

			result = procLock.Lock();

			if(false == result)
			{
				status = HOOK_NOREDIRECT;
			}
			else
			{
				*thunkFunctionOut = thunkFunctionPtr;

				//procAssembler.Jump(proxyFunction);
				procAssembler.JumpIndirect(thunkJumpPtr);

				//	TODO: Need a catalog to save it in if we want to properly clean up and/or uninstall the hook.
				//AddToCatalog(codeBuffer);
			}
		}

		VirtualAlloc(codeBuffer, THUNK_BUFFER_SIZE, MEM_COMMIT, PAGE_EXECUTE_READ);


		if(HOOK_SUCCESS != status)
		{
			HookError(moduleName, functionName, status);
		}


		//	Done
		return status;
	}

}}}
