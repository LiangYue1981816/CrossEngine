// ClearBufferMask
GL_DEPTH_BUFFER_BIT, "GL_DEPTH_BUFFER_BIT", 
GL_STENCIL_BUFFER_BIT, "GL_STENCIL_BUFFER_BIT",
GL_COLOR_BUFFER_BIT, "GL_COLOR_BUFFER_BIT", 

// Boolean
GL_TRUE, "GL_TRUE",
GL_TRUE, "TRUE", 
GL_TRUE, "1", 
GL_FALSE, "GL_FALSE", 
GL_FALSE, "FALSE", 
GL_FALSE, "0",  

// BeginMode
GL_POINTS, "GL_POINTS", 
GL_LINES, "GL_LINES", 
GL_LINE_LOOP, "GL_LINE_LOOP", 
GL_LINE_STRIP, "GL_LINE_STRIP", 
GL_TRIANGLES, "GL_TRIANGLES", 
GL_TRIANGLE_STRIP, "GL_TRIANGLE_STRIP", 
GL_TRIANGLE_FAN, "GL_TRIANGLE_FAN", 

// AlphaFunction (not supported in ES20)
// GL_NEVER
// GL_LESS
// GL_EQUAL
// GL_LEQUAL
// GL_GREATER
// GL_NOTEQUAL
// GL_GEQUAL
// GL_ALWAYS

// BlendingFactorDest
GL_ZERO, "GL_ZERO", 
GL_ONE, "GL_ONE", 
GL_SRC_COLOR, "GL_SRC_COLOR", 
GL_ONE_MINUS_SRC_COLOR, "GL_ONE_MINUS_SRC_COLOR", 
GL_SRC_ALPHA, "GL_SRC_ALPHA", 
GL_ONE_MINUS_SRC_ALPHA, "GL_ONE_MINUS_SRC_ALPHA", 
GL_DST_ALPHA, "GL_DST_ALPHA", 
GL_ONE_MINUS_DST_ALPHA, "GL_ONE_MINUS_DST_ALPHA", 

// BlendingFactorSrc
// GL_ZERO
// GL_ONE
GL_DST_COLOR, "GL_DST_COLOR", 
GL_ONE_MINUS_DST_COLOR, "GL_ONE_MINUS_DST_COLOR", 
GL_SRC_ALPHA_SATURATE, "GL_SRC_ALPHA_SATURATE", 
// GL_SRC_ALPHA
// GL_ONE_MINUS_SRC_ALPHA
// GL_DST_ALPHA
// GL_ONE_MINUS_DST_ALPHA

// BlendEquationSeparate
GL_FUNC_ADD, "GL_FUNC_ADD", 
GL_BLEND_EQUATION, "GL_BLEND_EQUATION", 
GL_BLEND_EQUATION_RGB, "GL_BLEND_EQUATION_RGB", 
GL_BLEND_EQUATION_ALPHA, "GL_BLEND_EQUATION_ALPHA", 

// BlendSubtract
GL_FUNC_SUBTRACT, "GL_FUNC_SUBTRACT", 
GL_FUNC_REVERSE_SUBTRACT, "GL_FUNC_REVERSE_SUBTRACT", 

// Separate Blend Functions
GL_BLEND_DST_RGB, "GL_BLEND_DST_RGB", 
GL_BLEND_SRC_RGB, "GL_BLEND_SRC_RGB", 
GL_BLEND_DST_ALPHA, "GL_BLEND_DST_ALPHA", 
GL_BLEND_SRC_ALPHA, "GL_BLEND_SRC_ALPHA", 
GL_CONSTANT_COLOR, "GL_CONSTANT_COLOR", 
GL_ONE_MINUS_CONSTANT_COLOR, "GL_ONE_MINUS_CONSTANT_COLOR", 
GL_CONSTANT_ALPHA, "GL_CONSTANT_ALPHA", 
GL_ONE_MINUS_CONSTANT_ALPHA, "GL_ONE_MINUS_CONSTANT_ALPHA", 
GL_BLEND_COLOR, "GL_BLEND_COLOR", 

// Buffer Objects
GL_ARRAY_BUFFER, "GL_ARRAY_BUFFER", 
GL_ELEMENT_ARRAY_BUFFER, "GL_ELEMENT_ARRAY_BUFFER", 
GL_ARRAY_BUFFER_BINDING, "GL_ARRAY_BUFFER_BINDING", 
GL_ELEMENT_ARRAY_BUFFER_BINDING, "GL_ELEMENT_ARRAY_BUFFER_BINDING", 

GL_STREAM_DRAW, "GL_STREAM_DRAW", 
GL_STATIC_DRAW, "GL_STATIC_DRAW", 
GL_DYNAMIC_DRAW, "GL_DYNAMIC_DRAW", 

GL_BUFFER_SIZE, "GL_BUFFER_SIZE", 
GL_BUFFER_USAGE, "GL_BUFFER_USAGE", 

GL_CURRENT_VERTEX_ATTRIB, "GL_CURRENT_VERTEX_ATTRIB", 

// CullFaceMode
GL_FRONT, "GL_FRONT", 
GL_BACK, "GL_BACK", 
GL_FRONT_AND_BACK, "GL_FRONT_AND_BACK", 

// DepthFunction
// GL_NEVER
// GL_LESS
// GL_EQUAL
// GL_LEQUAL
// GL_GREATER
// GL_NOTEQUAL
// GL_GEQUAL
// GL_ALWAYS

// EnableCap
GL_TEXTURE_2D, "GL_TEXTURE_2D", 
GL_CULL_FACE, "GL_CULL_FACE", 
GL_BLEND, "GL_BLEND", 
GL_DITHER, "GL_DITHER", 
GL_STENCIL_TEST, "GL_STENCIL_TEST", 
GL_DEPTH_TEST, "GL_DEPTH_TEST", 
GL_SCISSOR_TEST, "GL_SCISSOR_TEST", 
GL_POLYGON_OFFSET_FILL, "GL_POLYGON_OFFSET_FILL", 
GL_SAMPLE_ALPHA_TO_COVERAGE, "GL_SAMPLE_ALPHA_TO_COVERAGE", 
GL_SAMPLE_COVERAGE, "GL_SAMPLE_COVERAGE", 

// ErrorCode
GL_NO_ERROR, "GL_NO_ERROR", 
GL_INVALID_ENUM, "GL_INVALID_ENUM", 
GL_INVALID_VALUE, "GL_INVALID_VALUE", 
GL_INVALID_OPERATION, "GL_INVALID_OPERATION", 
GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY", 

// FrontFaceDirection
GL_CW, "GL_CW", 
GL_CCW, "GL_CCW", 

// GetPName
GL_LINE_WIDTH, "GL_LINE_WIDTH", 
GL_ALIASED_POINT_SIZE_RANGE, "GL_ALIASED_POINT_SIZE_RANGE", 
GL_ALIASED_LINE_WIDTH_RANGE, "GL_ALIASED_LINE_WIDTH_RANGE", 
GL_CULL_FACE_MODE, "GL_CULL_FACE_MODE", 
GL_FRONT_FACE, "GL_FRONT_FACE", 
GL_DEPTH_RANGE, "GL_DEPTH_RANGE", 
GL_DEPTH_WRITEMASK, "GL_DEPTH_WRITEMASK", 
GL_DEPTH_CLEAR_VALUE, "GL_DEPTH_CLEAR_VALUE", 
GL_DEPTH_FUNC, "GL_DEPTH_FUNC", 
GL_STENCIL_CLEAR_VALUE, "GL_STENCIL_CLEAR_VALUE", 
GL_STENCIL_FUNC, "GL_STENCIL_FUNC", 
GL_STENCIL_FAIL, "GL_STENCIL_FAIL", 
GL_STENCIL_PASS_DEPTH_FAIL, "GL_STENCIL_PASS_DEPTH_FAIL", 
GL_STENCIL_PASS_DEPTH_PASS, "GL_STENCIL_PASS_DEPTH_PASS", 
GL_STENCIL_REF, "GL_STENCIL_REF", 
GL_STENCIL_VALUE_MASK, "GL_STENCIL_VALUE_MASK", 
GL_STENCIL_WRITEMASK, "GL_STENCIL_WRITEMASK", 
GL_STENCIL_BACK_FUNC, "GL_STENCIL_BACK_FUNC", 
GL_STENCIL_BACK_FAIL, "GL_STENCIL_BACK_FAIL", 
GL_STENCIL_BACK_PASS_DEPTH_FAIL, "GL_STENCIL_BACK_PASS_DEPTH_FAIL", 
GL_STENCIL_BACK_PASS_DEPTH_PASS, "GL_STENCIL_BACK_PASS_DEPTH_PASS", 
GL_STENCIL_BACK_REF, "GL_STENCIL_BACK_REF", 
GL_STENCIL_BACK_VALUE_MASK, "GL_STENCIL_BACK_VALUE_MASK", 
GL_STENCIL_BACK_WRITEMASK, "GL_STENCIL_BACK_WRITEMASK", 
GL_VIEWPORT, "GL_VIEWPORT", 
GL_SCISSOR_BOX, "GL_SCISSOR_BOX", 
// GL_SCISSOR_TEST
GL_COLOR_CLEAR_VALUE, "GL_COLOR_CLEAR_VALUE", 
GL_COLOR_WRITEMASK, "GL_COLOR_WRITEMASK", 
GL_UNPACK_ALIGNMENT, "GL_UNPACK_ALIGNMENT", 
GL_PACK_ALIGNMENT, "GL_PACK_ALIGNMENT", 
GL_MAX_TEXTURE_SIZE, "GL_MAX_TEXTURE_SIZE", 
GL_MAX_VIEWPORT_DIMS, "GL_MAX_VIEWPORT_DIMS", 
GL_SUBPIXEL_BITS, "GL_SUBPIXEL_BITS", 
GL_RED_BITS, "GL_RED_BITS", 
GL_GREEN_BITS, "GL_GREEN_BITS", 
GL_BLUE_BITS, "GL_BLUE_BITS", 
GL_ALPHA_BITS, "GL_ALPHA_BITS", 
GL_DEPTH_BITS, "GL_DEPTH_BITS", 
GL_STENCIL_BITS, "GL_STENCIL_BITS", 
GL_POLYGON_OFFSET_UNITS, "GL_POLYGON_OFFSET_UNITS", 
// GL_POLYGON_OFFSET_FILL
GL_POLYGON_OFFSET_FACTOR, "GL_POLYGON_OFFSET_FACTOR", 
GL_TEXTURE_BINDING_2D, "GL_TEXTURE_BINDING_2D", 
GL_SAMPLE_BUFFERS, "GL_SAMPLE_BUFFERS", 
GL_SAMPLES, "GL_SAMPLES", 
GL_SAMPLE_COVERAGE_VALUE, "GL_SAMPLE_COVERAGE_VALUE", 
GL_SAMPLE_COVERAGE_INVERT, "GL_SAMPLE_COVERAGE_INVERT", 

// GetTextureParameter
// GL_TEXTURE_MAG_FILTER
// GL_TEXTURE_MIN_FILTER
// GL_TEXTURE_WRAP_S
// GL_TEXTURE_WRAP_T

GL_NUM_COMPRESSED_TEXTURE_FORMATS, "GL_NUM_COMPRESSED_TEXTURE_FORMATS", 
GL_COMPRESSED_TEXTURE_FORMATS, "GL_COMPRESSED_TEXTURE_FORMATS", 

// HintMode
GL_DONT_CARE, "GL_DONT_CARE", 
GL_FASTEST, "GL_FASTEST", 
GL_NICEST, "GL_NICEST", 

// HintTarget
GL_GENERATE_MIPMAP_HINT, "GL_GENERATE_MIPMAP_HINT", 

// DataType
GL_BYTE, "GL_BYTE", 
GL_UNSIGNED_BYTE, "GL_UNSIGNED_BYTE", 
GL_SHORT, "GL_SHORT", 
GL_UNSIGNED_SHORT, "GL_UNSIGNED_SHORT", 
GL_INT, "GL_INT", 
GL_UNSIGNED_INT, "GL_UNSIGNED_INT", 
GL_FLOAT, "GL_FLOAT", 
GL_FIXED, "GL_FIXED", 

// PixelFormat
GL_DEPTH_COMPONENT, "GL_DEPTH_COMPONENT", 
GL_ALPHA, "GL_ALPHA", 
GL_RGB, "GL_RGB", 
GL_RGBA, "GL_RGBA", 
GL_LUMINANCE, "GL_LUMINANCE", 
GL_LUMINANCE_ALPHA, "GL_LUMINANCE_ALPHA", 

// PixelType
// GL_UNSIGNED_BYTE
GL_UNSIGNED_SHORT_4_4_4_4, "GL_UNSIGNED_SHORT_4_4_4_4", 
GL_UNSIGNED_SHORT_5_5_5_1, "GL_UNSIGNED_SHORT_5_5_5_1", 
GL_UNSIGNED_SHORT_5_6_5, "GL_UNSIGNED_SHORT_5_6_5", 

// Shaders
GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER", 
GL_VERTEX_SHADER, "GL_VERTEX_SHADER", 
GL_MAX_VERTEX_ATTRIBS, "GL_MAX_VERTEX_ATTRIBS", 
GL_MAX_VERTEX_UNIFORM_VECTORS, "GL_MAX_VERTEX_UNIFORM_VECTORS", 
GL_MAX_VARYING_VECTORS, "GL_MAX_VARYING_VECTORS", 
GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", 
GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", 
GL_MAX_TEXTURE_IMAGE_UNITS, "GL_MAX_TEXTURE_IMAGE_UNITS", 
GL_MAX_FRAGMENT_UNIFORM_VECTORS, "GL_MAX_FRAGMENT_UNIFORM_VECTORS", 
GL_SHADER_TYPE, "GL_SHADER_TYPE", 
GL_DELETE_STATUS, "GL_DELETE_STATUS", 
GL_LINK_STATUS, "GL_LINK_STATUS", 
GL_VALIDATE_STATUS, "GL_VALIDATE_STATUS", 
GL_ATTACHED_SHADERS, "GL_ATTACHED_SHADERS", 
GL_ACTIVE_UNIFORMS, "GL_ACTIVE_UNIFORMS", 
GL_ACTIVE_UNIFORM_MAX_LENGTH, "GL_ACTIVE_UNIFORM_MAX_LENGTH", 
GL_ACTIVE_ATTRIBUTES, "GL_ACTIVE_ATTRIBUTES", 
GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, "GL_ACTIVE_ATTRIBUTE_MAX_LENGTH", 
GL_SHADING_LANGUAGE_VERSION, "GL_SHADING_LANGUAGE_VERSION", 
GL_CURRENT_PROGRAM, "GL_CURRENT_PROGRAM", 

// StencilFunction
GL_NEVER, "GL_NEVER", 
GL_LESS, "GL_LESS", 
GL_EQUAL, "GL_EQUAL", 
GL_LEQUAL, "GL_LEQUAL", 
GL_GREATER, "GL_GREATER", 
GL_NOTEQUAL, "GL_NOTEQUAL", 
GL_GEQUAL, "GL_GEQUAL", 
GL_ALWAYS, "GL_ALWAYS", 

// StencilOp
// GL_ZERO
GL_KEEP, "GL_KEEP", 
GL_REPLACE, "GL_REPLACE", 
GL_INCR, "GL_INCR", 
GL_DECR, "GL_DECR", 
GL_INVERT, "GL_INVERT", 
GL_INCR_WRAP, "GL_INCR_WRAP", 
GL_DECR_WRAP, "GL_DECR_WRAP", 

// StringName
GL_VENDOR, "GL_VENDOR", 
GL_RENDERER, "GL_RENDERER", 
GL_VERSION, "GL_VERSION", 
GL_EXTENSIONS, "GL_EXTENSIONS", 

// TextureMagFilter
GL_NEAREST, "GL_NEAREST", 
GL_LINEAR, "GL_LINEAR", 

// TextureMinFilter
// GL_NEAREST
// GL_LINEAR
GL_NEAREST_MIPMAP_NEAREST, "GL_NEAREST_MIPMAP_NEAREST", 
GL_LINEAR_MIPMAP_NEAREST, "GL_LINEAR_MIPMAP_NEAREST", 
GL_NEAREST_MIPMAP_LINEAR, "GL_NEAREST_MIPMAP_LINEAR", 
GL_LINEAR_MIPMAP_LINEAR, "GL_LINEAR_MIPMAP_LINEAR", 

// TextureParameterName
GL_TEXTURE_MAG_FILTER, "GL_TEXTURE_MAG_FILTER", 
GL_TEXTURE_MIN_FILTER, "GL_TEXTURE_MIN_FILTER", 
GL_TEXTURE_WRAP_S, "GL_TEXTURE_WRAP_S", 
GL_TEXTURE_WRAP_T, "GL_TEXTURE_WRAP_T", 

// TextureTarget
// GL_TEXTURE_2D
GL_TEXTURE, "GL_TEXTURE", 

GL_TEXTURE_CUBE_MAP, "GL_TEXTURE_CUBE_MAP", 
GL_TEXTURE_BINDING_CUBE_MAP, "GL_TEXTURE_BINDING_CUBE_MAP", 
GL_TEXTURE_CUBE_MAP_POSITIVE_X, "GL_TEXTURE_CUBE_MAP_POSITIVE_X", 
GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "GL_TEXTURE_CUBE_MAP_NEGATIVE_X", 
GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "GL_TEXTURE_CUBE_MAP_POSITIVE_Y", 
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "GL_TEXTURE_CUBE_MAP_NEGATIVE_Y", 
GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "GL_TEXTURE_CUBE_MAP_POSITIVE_Z", 
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "GL_TEXTURE_CUBE_MAP_NEGATIVE_Z", 
GL_MAX_CUBE_MAP_TEXTURE_SIZE, "GL_MAX_CUBE_MAP_TEXTURE_SIZE", 

// TextureUnit
GL_TEXTURE0, "GL_TEXTURE0",
GL_TEXTURE1, "GL_TEXTURE1", 
GL_TEXTURE2, "GL_TEXTURE2", 
GL_TEXTURE3, "GL_TEXTURE3", 
GL_TEXTURE4, "GL_TEXTURE4", 
GL_TEXTURE5, "GL_TEXTURE5", 
GL_TEXTURE6, "GL_TEXTURE6", 
GL_TEXTURE7, "GL_TEXTURE7", 
GL_TEXTURE8, "GL_TEXTURE8", 
GL_TEXTURE9, "GL_TEXTURE9", 
GL_TEXTURE10, "GL_TEXTURE10", 
GL_TEXTURE11, "GL_TEXTURE11", 
GL_TEXTURE12, "GL_TEXTURE12", 
GL_TEXTURE13, "GL_TEXTURE13", 
GL_TEXTURE14, "GL_TEXTURE14", 
GL_TEXTURE15, "GL_TEXTURE15", 
GL_TEXTURE16, "GL_TEXTURE16", 
GL_TEXTURE17, "GL_TEXTURE17", 
GL_TEXTURE18, "GL_TEXTURE18", 
GL_TEXTURE19, "GL_TEXTURE19", 
GL_TEXTURE20, "GL_TEXTURE20", 
GL_TEXTURE21, "GL_TEXTURE21", 
GL_TEXTURE22, "GL_TEXTURE22", 
GL_TEXTURE23, "GL_TEXTURE23", 
GL_TEXTURE24, "GL_TEXTURE24", 
GL_TEXTURE25, "GL_TEXTURE25", 
GL_TEXTURE26, "GL_TEXTURE26", 
GL_TEXTURE27, "GL_TEXTURE27", 
GL_TEXTURE28, "GL_TEXTURE28", 
GL_TEXTURE29, "GL_TEXTURE29", 
GL_TEXTURE30, "GL_TEXTURE30", 
GL_TEXTURE31, "GL_TEXTURE31", 
GL_ACTIVE_TEXTURE, "GL_ACTIVE_TEXTURE", 

// TextureWrapMode
GL_REPEAT, "GL_REPEAT", 
GL_CLAMP_TO_EDGE, "GL_CLAMP", 
GL_CLAMP_TO_EDGE, "GL_CLAMP_TO_EDGE", 
GL_MIRRORED_REPEAT, "GL_MIRRORED_REPEAT", 

// Uniform Types
GL_FLOAT_VEC2, "GL_FLOAT_VEC2", 
GL_FLOAT_VEC3, "GL_FLOAT_VEC3", 
GL_FLOAT_VEC4, "GL_FLOAT_VEC4", 
GL_INT_VEC2, "GL_INT_VEC2", 
GL_INT_VEC3, "GL_INT_VEC3", 
GL_INT_VEC4, "GL_INT_VEC4", 
GL_BOOL, "GL_BOOL", 
GL_BOOL_VEC2, "GL_BOOL_VEC2", 
GL_BOOL_VEC3, "GL_BOOL_VEC3", 
GL_BOOL_VEC4, "GL_BOOL_VEC4", 
GL_FLOAT_MAT2, "GL_FLOAT_MAT2", 
GL_FLOAT_MAT3, "GL_FLOAT_MAT3", 
GL_FLOAT_MAT4, "GL_FLOAT_MAT4", 
GL_SAMPLER_2D, "GL_SAMPLER_2D", 
GL_SAMPLER_CUBE, "GL_SAMPLER_CUBE", 

// Vertex Arrays
GL_VERTEX_ATTRIB_ARRAY_ENABLED, "GL_VERTEX_ATTRIB_ARRAY_ENABLED", 
GL_VERTEX_ATTRIB_ARRAY_SIZE, "GL_VERTEX_ATTRIB_ARRAY_SIZE", 
GL_VERTEX_ATTRIB_ARRAY_STRIDE, "GL_VERTEX_ATTRIB_ARRAY_STRIDE", 
GL_VERTEX_ATTRIB_ARRAY_TYPE, "GL_VERTEX_ATTRIB_ARRAY_TYPE", 
GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED", 
GL_VERTEX_ATTRIB_ARRAY_POINTER, "GL_VERTEX_ATTRIB_ARRAY_POINTER", 
GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING", 

// Read Format
GL_IMPLEMENTATION_COLOR_READ_TYPE, "GL_IMPLEMENTATION_COLOR_READ_TYPE", 
GL_IMPLEMENTATION_COLOR_READ_FORMAT, "GL_IMPLEMENTATION_COLOR_READ_FORMAT", 

// Shader Source
GL_COMPILE_STATUS, "GL_COMPILE_STATUS", 
GL_INFO_LOG_LENGTH, "GL_INFO_LOG_LENGTH", 
GL_SHADER_SOURCE_LENGTH, "GL_SHADER_SOURCE_LENGTH", 
GL_SHADER_COMPILER, "GL_SHADER_COMPILER", 

// Shader Binary
GL_SHADER_BINARY_FORMATS, "GL_SHADER_BINARY_FORMATS",
GL_NUM_SHADER_BINARY_FORMATS, "GL_NUM_SHADER_BINARY_FORMATS", 

// Shader Precision-Specified Types
GL_LOW_FLOAT, "GL_LOW_FLOAT", 
GL_MEDIUM_FLOAT, "GL_MEDIUM_FLOAT", 
GL_HIGH_FLOAT, "GL_HIGH_FLOAT", 
GL_LOW_INT, "GL_LOW_INT", 
GL_MEDIUM_INT, "GL_MEDIUM_INT", 
GL_HIGH_INT, "GL_HIGH_INT", 

// Framebuffer Object.
GL_FRAMEBUFFER, "GL_FRAMEBUFFER", 
GL_RENDERBUFFER, "GL_RENDERBUFFER", 

GL_RGBA4, "GL_RGBA4", 
GL_RGB5_A1, "GL_RGB5_A1", 
GL_RGB565, "GL_RGB565", 
GL_DEPTH_COMPONENT16, "GL_DEPTH_COMPONENT16", 
GL_STENCIL_INDEX, "GL_STENCIL_INDEX", 
GL_STENCIL_INDEX8, "GL_STENCIL_INDEX8", 

GL_RENDERBUFFER_WIDTH, "GL_RENDERBUFFER_WIDTH", 
GL_RENDERBUFFER_HEIGHT, "GL_RENDERBUFFER_HEIGHT", 
GL_RENDERBUFFER_INTERNAL_FORMAT, "GL_RENDERBUFFER_INTERNAL_FORMAT", 
GL_RENDERBUFFER_RED_SIZE, "GL_RENDERBUFFER_RED_SIZE", 
GL_RENDERBUFFER_GREEN_SIZE, "GL_RENDERBUFFER_GREEN_SIZE", 
GL_RENDERBUFFER_BLUE_SIZE, "GL_RENDERBUFFER_BLUE_SIZE", 
GL_RENDERBUFFER_ALPHA_SIZE, "GL_RENDERBUFFER_ALPHA_SIZE", 
GL_RENDERBUFFER_DEPTH_SIZE, "GL_RENDERBUFFER_DEPTH_SIZE", 
GL_RENDERBUFFER_STENCIL_SIZE, "GL_RENDERBUFFER_STENCIL_SIZE", 

GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE", 
GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME", 
GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL, "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL", 
GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE, "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE", 

GL_COLOR_ATTACHMENT0, "GL_COLOR_ATTACHMENT0", 
GL_DEPTH_ATTACHMENT, "GL_DEPTH_ATTACHMENT", 
GL_STENCIL_ATTACHMENT, "GL_STENCIL_ATTACHMENT", 

GL_NONE, "GL_NONE", 

GL_FRAMEBUFFER_COMPLETE, "GL_FRAMEBUFFER_COMPLETE", 
GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT", 
GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", 
// GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS, "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS", 
GL_FRAMEBUFFER_UNSUPPORTED, "GL_FRAMEBUFFER_UNSUPPORTED", 

GL_FRAMEBUFFER_BINDING, "GL_FRAMEBUFFER_BINDING", 
GL_RENDERBUFFER_BINDING, "GL_RENDERBUFFER_BINDING", 
GL_MAX_RENDERBUFFER_SIZE, "GL_MAX_RENDERBUFFER_SIZE", 

GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION", 