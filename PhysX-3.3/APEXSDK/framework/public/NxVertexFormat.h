/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */


#ifndef NX_VERTEX_FORMAT_H
#define NX_VERTEX_FORMAT_H

/*!
\file
\brief class NxVertexFormat and struct NxVertexFormatFlag
*/

#include "NxApexUsingNamespace.h"
#include "NxUserRenderResourceManager.h"
#include "NxUserRenderVertexBufferDesc.h"

namespace physx
{
namespace apex
{

PX_PUSH_PACK_DEFAULT

/**
\brief Color stored in 32 bits
*/
struct PxColorRGBA
{
	///Constructor
	PxColorRGBA() : r(0xFF), g(0xFF), b(0xFF), a(0xFF) {}
	///Constructor
	PxColorRGBA(PxU8 _r, PxU8 _g, PxU8 _b, PxU8 _a) : r(_r), g(_g), b(_b), a(_a) {}

	///color
	PxU8	r;
	///color
	PxU8	g;
	///color
	PxU8	b;
	///color
	PxU8	a;
};

/**
\brief Type of render data
*/
struct NxRenderDataAccess
{
	/**
	\brief Enum of type of render data
	*/
	enum Enum
	{
		STATIC = 0,
		DYNAMIC,
		STREAMING,

		ACCESS_TYPE_COUNT
	};
};

/**
\brief Describes the format of an NxVertexBuffer.
*/
class NxVertexFormat
{
public:

	enum
	{
		MAX_UV_COUNT = 4,
		MAX_BONE_PER_VERTEX_COUNT = 4,
	};

	/** \brief Buffer ID */
	typedef physx::PxU32 BufferID;


	/** \brief Resets the format to the initial state */
	virtual void							reset() = 0;


	/** \brief Sets the winding (cull mode) for this format */
	virtual void							setWinding(NxRenderCullMode::Enum winding) = 0;

	/** \brief Sets whether or not a separate bone buffer is used */
	virtual void							setHasSeparateBoneBuffer(bool hasSeparateBoneBuffer) = 0;

	/** \brief Accessor to read winding (cull mode) */
	virtual NxRenderCullMode::Enum			getWinding() const = 0;

	/** \brief Accessor to read if a seperate vertex buffer for bone indices and weights is generated */
	virtual bool							hasSeparateBoneBuffer() const = 0;


	/** \brief Returns a buffer name for a semantic. Returns NULL if the semantic is invalid */
	virtual const char*						getSemanticName(NxRenderVertexSemantic::Enum semantic) const = 0;

	/** \brief Returns a buffer ID for a semantic. For custom buffers, use the getID() function. */
	virtual BufferID						getSemanticID(NxRenderVertexSemantic::Enum semantic) const = 0;

	/** \brief Returns a buffer ID for a named buffer. For standard semantics, the getSemanticID( semantic ) function is faster, but
	is equivalent to getID( getSemanticName( semantic ) ). Returns 0 if name == NULL */
	virtual BufferID						getID(const char* name) const = 0;


	/** \brief Adds a vertex buffer channel to this format
	\param [in] name the name of a new buffer (use getSemanticName for standard semantics)
	\return The buffer index. If the buffer for the semantic already exists, the index of the existing buffer is returned. Returns -1 if there is an error (e.g. name == NULL).
	*/
	virtual physx::PxI32					addBuffer(const char* name) = 0;

	/** \brief Removes a buffer
	\param [in] index the buffer to remove
	\return True if successful, false otherwise (if the buffer index was invalid)
	*/
	virtual bool							bufferReplaceWithLast(physx::PxU32 index) = 0;


	/** \brief Set the format for a semantic
	\return True if successful, false otherwise (if the buffer index was invalid)
	*/
	virtual bool							setBufferFormat(physx::PxU32 index, NxRenderDataFormat::Enum format) = 0;

	/** \brief Set the access type for a buffer (static, dynamic, etc.)
	\return True if successful, false otherwise (if the buffer index was invalid)
	*/
	virtual bool							setBufferAccess(physx::PxU32 index, NxRenderDataAccess::Enum access) = 0;

	/** \brief Set whether or not the buffer should be serialized
	\return True if successful, false otherwise (if the buffer index was invalid)
	*/
	virtual bool							setBufferSerialize(physx::PxU32 index, bool serialize) = 0;


	/** \brief Accessor to read the name of a given buffer
	\return The buffer name if successful, NULL otherwise.
	*/
	virtual const char*						getBufferName(physx::PxU32 index) const = 0;

	/** \brief Accessor to read the semantic of a given buffer
	\return The buffer semantic if successful, NxRenderVertexSemantic::NUM_SEMANTICS otherwise.
	*/
	virtual NxRenderVertexSemantic::Enum	getBufferSemantic(physx::PxU32 index) const = 0;

	/** \brief Accessor to read the ID of a given buffer
	\return The buffer semantic if successful, 0 otherwise.
	*/
	virtual BufferID						getBufferID(physx::PxU32 index) const = 0;

	/** \brief Get the format for a buffer
	\return The buffer format if successful, NxRenderDataFormat::UNSPECIFIED otherwise.
	*/
	virtual NxRenderDataFormat::Enum		getBufferFormat(physx::PxU32 index) const = 0;

	/** \brief Get the access type for a buffer (static, dynamic, etc.)
	\return The buffer access if successful, NxRenderDataAccess::ACCESS_TYPE_COUNT otherwise.
	*/
	virtual NxRenderDataAccess::Enum		getBufferAccess(physx::PxU32 index) const = 0;

	/** \brief Get whether or not the buffer should be serialized
	\return Whether or not the buffer should be serialized if successful, false otherwise.
	*/
	virtual bool							getBufferSerialize(physx::PxU32 index) const = 0;


	/** \brief Accessor to read the number of buffers */
	virtual physx::PxU32					getBufferCount() const = 0;

	/** \brief Returns the number of buffers that are user-specified */
	virtual physx::PxU32					getCustomBufferCount() const = 0;

	/** \brief Accessor to get the buffer index
	If the buffer is not found, -1 is returned
	*/
	virtual physx::PxI32					getBufferIndexFromID(BufferID id) const = 0;


protected:
	NxVertexFormat()	{}
	virtual									~NxVertexFormat()	{}
};

PX_POP_PACK

}
} // end namespace physx::apex

#endif // NX_VERTEX_FORMAT_H
