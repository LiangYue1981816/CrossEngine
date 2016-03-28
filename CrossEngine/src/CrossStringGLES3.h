// OpenGL ES 2.0

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

// OpenGL ES 3.0
GL_READ_BUFFER, "GL_READ_BUFFER", 
GL_UNPACK_ROW_LENGTH, "GL_UNPACK_ROW_LENGTH", 
GL_UNPACK_SKIP_ROWS, "GL_UNPACK_SKIP_ROWS", 
GL_UNPACK_SKIP_PIXELS, "GL_UNPACK_SKIP_PIXELS", 
GL_PACK_ROW_LENGTH, "GL_PACK_ROW_LENGTH", 
GL_PACK_SKIP_ROWS, "GL_PACK_SKIP_ROWS", 
GL_PACK_SKIP_PIXELS, "GL_PACK_SKIP_PIXELS", 
GL_COLOR, "GL_COLOR", 
GL_DEPTH, "GL_DEPTH", 
GL_STENCIL, "GL_STENCIL", 
GL_RED, "GL_RED", 
GL_RGB8, "GL_RGB8", 
GL_RGBA8, "GL_RGBA8", 
GL_RGB10_A2, "GL_RGB10_A2", 
GL_TEXTURE_BINDING_3D, "GL_TEXTURE_BINDING_3D", 
GL_UNPACK_SKIP_IMAGES, "GL_UNPACK_SKIP_IMAGES", 
GL_UNPACK_IMAGE_HEIGHT, "GL_UNPACK_IMAGE_HEIGHT", 
GL_TEXTURE_3D, "GL_TEXTURE_3D", 
GL_TEXTURE_WRAP_R, "GL_TEXTURE_WRAP_R", 
GL_MAX_3D_TEXTURE_SIZE, "GL_MAX_3D_TEXTURE_SIZE", 
GL_UNSIGNED_INT_2_10_10_10_REV, "GL_UNSIGNED_INT_2_10_10_10_REV", 
GL_MAX_ELEMENTS_VERTICES, "GL_MAX_ELEMENTS_VERTICES", 
GL_MAX_ELEMENTS_INDICES, "GL_MAX_ELEMENTS_INDICES", 
GL_TEXTURE_MIN_LOD, "GL_TEXTURE_MIN_LOD", 
GL_TEXTURE_MAX_LOD, "GL_TEXTURE_MAX_LOD", 
GL_TEXTURE_BASE_LEVEL, "GL_TEXTURE_BASE_LEVEL", 
GL_TEXTURE_MAX_LEVEL, "GL_TEXTURE_MAX_LEVEL", 
GL_MIN, "GL_MIN", 
GL_MAX, "GL_MAX", 
GL_DEPTH_COMPONENT24, "GL_DEPTH_COMPONENT24", 
GL_MAX_TEXTURE_LOD_BIAS, "GL_MAX_TEXTURE_LOD_BIAS", 
GL_TEXTURE_COMPARE_MODE, "GL_TEXTURE_COMPARE_MODE", 
GL_TEXTURE_COMPARE_FUNC, "GL_TEXTURE_COMPARE_FUNC", 
GL_CURRENT_QUERY, "GL_CURRENT_QUERY", 
GL_QUERY_RESULT, "GL_QUERY_RESULT", 
GL_QUERY_RESULT_AVAILABLE, "GL_QUERY_RESULT_AVAILABLE", 
GL_BUFFER_MAPPED, "GL_BUFFER_MAPPED", 
GL_BUFFER_MAP_POINTER, "GL_BUFFER_MAP_POINTER", 
GL_STREAM_READ, "GL_STREAM_READ", 
GL_STREAM_COPY, "GL_STREAM_COPY", 
GL_STATIC_READ, "GL_STATIC_READ", 
GL_STATIC_COPY, "GL_STATIC_COPY", 
GL_DYNAMIC_READ, "GL_DYNAMIC_READ", 
GL_DYNAMIC_COPY, "GL_DYNAMIC_COPY", 
GL_MAX_DRAW_BUFFERS, "GL_MAX_DRAW_BUFFERS", 
GL_DRAW_BUFFER0, "GL_DRAW_BUFFER0", 
GL_DRAW_BUFFER1, "GL_DRAW_BUFFER1", 
GL_DRAW_BUFFER2, "GL_DRAW_BUFFER2", 
GL_DRAW_BUFFER3, "GL_DRAW_BUFFER3", 
GL_DRAW_BUFFER4, "GL_DRAW_BUFFER4", 
GL_DRAW_BUFFER5, "GL_DRAW_BUFFER5", 
GL_DRAW_BUFFER6, "GL_DRAW_BUFFER6", 
GL_DRAW_BUFFER7, "GL_DRAW_BUFFER7", 
GL_DRAW_BUFFER8, "GL_DRAW_BUFFER8", 
GL_DRAW_BUFFER9, "GL_DRAW_BUFFER9", 
GL_DRAW_BUFFER10, "GL_DRAW_BUFFER10", 
GL_DRAW_BUFFER11, "GL_DRAW_BUFFER11", 
GL_DRAW_BUFFER12, "GL_DRAW_BUFFER12", 
GL_DRAW_BUFFER13, "GL_DRAW_BUFFER13", 
GL_DRAW_BUFFER14, "GL_DRAW_BUFFER14", 
GL_DRAW_BUFFER15, "GL_DRAW_BUFFER15", 
GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS", 
GL_MAX_VERTEX_UNIFORM_COMPONENTS, "GL_MAX_VERTEX_UNIFORM_COMPONENTS", 
GL_SAMPLER_3D, "GL_SAMPLER_3D", 
GL_SAMPLER_2D_SHADOW, "GL_SAMPLER_2D_SHADOW", 
GL_FRAGMENT_SHADER_DERIVATIVE_HINT, "GL_FRAGMENT_SHADER_DERIVATIVE_HINT", 
GL_PIXEL_PACK_BUFFER, "GL_PIXEL_PACK_BUFFER", 
GL_PIXEL_UNPACK_BUFFER, "GL_PIXEL_UNPACK_BUFFER", 
GL_PIXEL_PACK_BUFFER_BINDING, "GL_PIXEL_PACK_BUFFER_BINDING", 
GL_PIXEL_UNPACK_BUFFER_BINDING, "GL_PIXEL_UNPACK_BUFFER_BINDING", 
GL_FLOAT_MAT2x3, "GL_FLOAT_MAT2x3", 
GL_FLOAT_MAT2x4, "GL_FLOAT_MAT2x4", 
GL_FLOAT_MAT3x2, "GL_FLOAT_MAT3x2", 
GL_FLOAT_MAT3x4, "GL_FLOAT_MAT3x4", 
GL_FLOAT_MAT4x2, "GL_FLOAT_MAT4x2", 
GL_FLOAT_MAT4x3, "GL_FLOAT_MAT4x3", 
GL_SRGB, "GL_SRGB", 
GL_SRGB8, "GL_SRGB8", 
GL_SRGB8_ALPHA8, "GL_SRGB8_ALPHA8", 
GL_COMPARE_REF_TO_TEXTURE, "GL_COMPARE_REF_TO_TEXTURE", 
GL_MAJOR_VERSION, "GL_MAJOR_VERSION", 
GL_MINOR_VERSION, "GL_MINOR_VERSION", 
GL_NUM_EXTENSIONS, "GL_NUM_EXTENSIONS", 
GL_RGBA32F, "GL_RGBA32F", 
GL_RGB32F, "GL_RGB32F", 
GL_RGBA16F, "GL_RGBA16F", 
GL_RGB16F, "GL_RGB16F", 
GL_VERTEX_ATTRIB_ARRAY_INTEGER, "GL_VERTEX_ATTRIB_ARRAY_INTEGER", 
GL_MAX_ARRAY_TEXTURE_LAYERS, "GL_MAX_ARRAY_TEXTURE_LAYERS", 
GL_MIN_PROGRAM_TEXEL_OFFSET, "GL_MIN_PROGRAM_TEXEL_OFFSET", 
GL_MAX_PROGRAM_TEXEL_OFFSET, "GL_MAX_PROGRAM_TEXEL_OFFSET", 
GL_MAX_VARYING_COMPONENTS, "GL_MAX_VARYING_COMPONENTS", 
GL_TEXTURE_2D_ARRAY, "GL_TEXTURE_2D_ARRAY", 
GL_TEXTURE_BINDING_2D_ARRAY, "GL_TEXTURE_BINDING_2D_ARRAY", 
GL_R11F_G11F_B10F, "GL_R11F_G11F_B10F", 
GL_UNSIGNED_INT_10F_11F_11F_REV, "GL_UNSIGNED_INT_10F_11F_11F_REV", 
GL_RGB9_E5, "GL_RGB9_E5", 
GL_UNSIGNED_INT_5_9_9_9_REV, "GL_UNSIGNED_INT_5_9_9_9_REV", 
GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH, "GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH", 
GL_TRANSFORM_FEEDBACK_BUFFER_MODE, "GL_TRANSFORM_FEEDBACK_BUFFER_MODE", 
GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS, "GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS", 
GL_TRANSFORM_FEEDBACK_VARYINGS, "GL_TRANSFORM_FEEDBACK_VARYINGS", 
GL_TRANSFORM_FEEDBACK_BUFFER_START, "GL_TRANSFORM_FEEDBACK_BUFFER_START", 
GL_TRANSFORM_FEEDBACK_BUFFER_SIZE, "GL_TRANSFORM_FEEDBACK_BUFFER_SIZE", 
GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, "GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN", 
GL_RASTERIZER_DISCARD, "GL_RASTERIZER_DISCARD", 
GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS, "GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS", 
GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, "GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS", 
GL_INTERLEAVED_ATTRIBS, "GL_INTERLEAVED_ATTRIBS", 
GL_SEPARATE_ATTRIBS, "GL_SEPARATE_ATTRIBS", 
GL_TRANSFORM_FEEDBACK_BUFFER, "GL_TRANSFORM_FEEDBACK_BUFFER", 
GL_TRANSFORM_FEEDBACK_BUFFER_BINDING, "GL_TRANSFORM_FEEDBACK_BUFFER_BINDING", 
GL_RGBA32UI, "GL_RGBA32UI", 
GL_RGB32UI, "GL_RGB32UI", 
GL_RGBA16UI, "GL_RGBA16UI", 
GL_RGB16UI, "GL_RGB16UI", 
GL_RGBA8UI, "GL_RGBA8UI", 
GL_RGB8UI, "GL_RGB8UI", 
GL_RGBA32I, "GL_RGBA32I", 
GL_RGB32I, "GL_RGB32I", 
GL_RGBA16I, "GL_RGBA16I", 
GL_RGB16I, "GL_RGB16I", 
GL_RGBA8I, "GL_RGBA8I", 
GL_RGB8I, "GL_RGB8I", 
GL_RED_INTEGER, "GL_RED_INTEGER", 
GL_RGB_INTEGER, "GL_RGB_INTEGER", 
GL_RGBA_INTEGER, "GL_RGBA_INTEGER", 
GL_SAMPLER_2D_ARRAY, "GL_SAMPLER_2D_ARRAY", 
GL_SAMPLER_2D_ARRAY_SHADOW, "GL_SAMPLER_2D_ARRAY_SHADOW", 
GL_SAMPLER_CUBE_SHADOW, "GL_SAMPLER_CUBE_SHADOW", 
GL_UNSIGNED_INT_VEC2, "GL_UNSIGNED_INT_VEC2", 
GL_UNSIGNED_INT_VEC3, "GL_UNSIGNED_INT_VEC3", 
GL_UNSIGNED_INT_VEC4, "GL_UNSIGNED_INT_VEC4", 
GL_INT_SAMPLER_2D, "GL_INT_SAMPLER_2D", 
GL_INT_SAMPLER_3D, "GL_INT_SAMPLER_3D", 
GL_INT_SAMPLER_CUBE, "GL_INT_SAMPLER_CUBE", 
GL_INT_SAMPLER_2D_ARRAY, "GL_INT_SAMPLER_2D_ARRAY", 
GL_UNSIGNED_INT_SAMPLER_2D, "GL_UNSIGNED_INT_SAMPLER_2D", 
GL_UNSIGNED_INT_SAMPLER_3D, "GL_UNSIGNED_INT_SAMPLER_3D", 
GL_UNSIGNED_INT_SAMPLER_CUBE, "GL_UNSIGNED_INT_SAMPLER_CUBE", 
GL_UNSIGNED_INT_SAMPLER_2D_ARRAY, "GL_UNSIGNED_INT_SAMPLER_2D_ARRAY", 
GL_BUFFER_ACCESS_FLAGS, "GL_BUFFER_ACCESS_FLAGS", 
GL_BUFFER_MAP_LENGTH, "GL_BUFFER_MAP_LENGTH", 
GL_BUFFER_MAP_OFFSET, "GL_BUFFER_MAP_OFFSET", 
GL_DEPTH_COMPONENT32F, "GL_DEPTH_COMPONENT32F", 
GL_DEPTH32F_STENCIL8, "GL_DEPTH32F_STENCIL8", 
GL_FLOAT_32_UNSIGNED_INT_24_8_REV, "GL_FLOAT_32_UNSIGNED_INT_24_8_REV", 
GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING, "GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING", 
GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE, "GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE", 
GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, "GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE", 
GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, "GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE", 
GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, "GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE", 
GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, "GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE", 
GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, "GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE", 
GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, "GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE", 
GL_FRAMEBUFFER_DEFAULT, "GL_FRAMEBUFFER_DEFAULT", 
GL_FRAMEBUFFER_UNDEFINED, "GL_FRAMEBUFFER_UNDEFINED", 
GL_DEPTH_STENCIL_ATTACHMENT, "GL_DEPTH_STENCIL_ATTACHMENT", 
GL_DEPTH_STENCIL, "GL_DEPTH_STENCIL", 
GL_UNSIGNED_INT_24_8, "GL_UNSIGNED_INT_24_8", 
GL_DEPTH24_STENCIL8, "GL_DEPTH24_STENCIL8", 
GL_UNSIGNED_NORMALIZED, "GL_UNSIGNED_NORMALIZED", 
GL_DRAW_FRAMEBUFFER_BINDING, "GL_DRAW_FRAMEBUFFER_BINDING", 
GL_READ_FRAMEBUFFER, "GL_READ_FRAMEBUFFER", 
GL_DRAW_FRAMEBUFFER, "GL_DRAW_FRAMEBUFFER", 
GL_READ_FRAMEBUFFER_BINDING, "GL_READ_FRAMEBUFFER_BINDING", 
GL_RENDERBUFFER_SAMPLES, "GL_RENDERBUFFER_SAMPLES", 
GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER, "GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER", 
GL_MAX_COLOR_ATTACHMENTS, "GL_MAX_COLOR_ATTACHMENTS", 
GL_COLOR_ATTACHMENT1, "GL_COLOR_ATTACHMENT1", 
GL_COLOR_ATTACHMENT2, "GL_COLOR_ATTACHMENT2", 
GL_COLOR_ATTACHMENT3, "GL_COLOR_ATTACHMENT3", 
GL_COLOR_ATTACHMENT4, "GL_COLOR_ATTACHMENT4", 
GL_COLOR_ATTACHMENT5, "GL_COLOR_ATTACHMENT5", 
GL_COLOR_ATTACHMENT6, "GL_COLOR_ATTACHMENT6", 
GL_COLOR_ATTACHMENT7, "GL_COLOR_ATTACHMENT7", 
GL_COLOR_ATTACHMENT8, "GL_COLOR_ATTACHMENT8", 
GL_COLOR_ATTACHMENT9, "GL_COLOR_ATTACHMENT9", 
GL_COLOR_ATTACHMENT10, "GL_COLOR_ATTACHMENT10", 
GL_COLOR_ATTACHMENT11, "GL_COLOR_ATTACHMENT11", 
GL_COLOR_ATTACHMENT12, "GL_COLOR_ATTACHMENT12", 
GL_COLOR_ATTACHMENT13, "GL_COLOR_ATTACHMENT13", 
GL_COLOR_ATTACHMENT14, "GL_COLOR_ATTACHMENT14", 
GL_COLOR_ATTACHMENT15, "GL_COLOR_ATTACHMENT15", 
GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE", 
GL_MAX_SAMPLES, "GL_MAX_SAMPLES", 
GL_HALF_FLOAT, "GL_HALF_FLOAT", 
GL_MAP_READ_BIT, "GL_MAP_READ_BIT", 
GL_MAP_WRITE_BIT, "GL_MAP_WRITE_BIT", 
GL_MAP_INVALIDATE_RANGE_BIT, "GL_MAP_INVALIDATE_RANGE_BIT", 
GL_MAP_INVALIDATE_BUFFER_BIT, "GL_MAP_INVALIDATE_BUFFER_BIT", 
GL_MAP_FLUSH_EXPLICIT_BIT, "GL_MAP_FLUSH_EXPLICIT_BIT", 
GL_MAP_UNSYNCHRONIZED_BIT, "GL_MAP_UNSYNCHRONIZED_BIT", 
GL_RG, "GL_RG", 
GL_RG_INTEGER, "GL_RG_INTEGER", 
GL_R8, "GL_R8", 
GL_RG8, "GL_RG8", 
GL_R16F, "GL_R16F", 
GL_R32F, "GL_R32F", 
GL_RG16F, "GL_RG16F", 
GL_RG32F, "GL_RG32F", 
GL_R8I, "GL_R8I", 
GL_R8UI, "GL_R8UI", 
GL_R16I, "GL_R16I", 
GL_R16UI, "GL_R16UI", 
GL_R32I, "GL_R32I", 
GL_R32UI, "GL_R32UI", 
GL_RG8I, "GL_RG8I", 
GL_RG8UI, "GL_RG8UI", 
GL_RG16I, "GL_RG16I", 
GL_RG16UI, "GL_RG16UI", 
GL_RG32I, "GL_RG32I", 
GL_RG32UI, "GL_RG32UI", 
GL_VERTEX_ARRAY_BINDING, "GL_VERTEX_ARRAY_BINDING", 
GL_R8_SNORM, "GL_R8_SNORM", 
GL_RG8_SNORM, "GL_RG8_SNORM", 
GL_RGB8_SNORM, "GL_RGB8_SNORM", 
GL_RGBA8_SNORM, "GL_RGBA8_SNORM", 
GL_SIGNED_NORMALIZED, "GL_SIGNED_NORMALIZED", 
//GL_PRIMITIVE_RESTART_FIXED_INDEX, "GL_PRIMITIVE_RESTART_FIXED_INDEX", 
GL_COPY_READ_BUFFER, "GL_COPY_READ_BUFFER", 
GL_COPY_WRITE_BUFFER, "GL_COPY_WRITE_BUFFER", 
//GL_COPY_READ_BUFFER_BINDING, "GL_COPY_READ_BUFFER_BINDING", 
//GL_COPY_WRITE_BUFFER_BINDING, "GL_COPY_WRITE_BUFFER_BINDING", 
GL_UNIFORM_BUFFER, "GL_UNIFORM_BUFFER", 
GL_UNIFORM_BUFFER_BINDING, "GL_UNIFORM_BUFFER_BINDING", 
GL_UNIFORM_BUFFER_START, "GL_UNIFORM_BUFFER_START", 
GL_UNIFORM_BUFFER_SIZE, "GL_UNIFORM_BUFFER_SIZE", 
GL_MAX_VERTEX_UNIFORM_BLOCKS, "GL_MAX_VERTEX_UNIFORM_BLOCKS", 
GL_MAX_FRAGMENT_UNIFORM_BLOCKS, "GL_MAX_FRAGMENT_UNIFORM_BLOCKS", 
GL_MAX_COMBINED_UNIFORM_BLOCKS, "GL_MAX_COMBINED_UNIFORM_BLOCKS", 
GL_MAX_UNIFORM_BUFFER_BINDINGS, "GL_MAX_UNIFORM_BUFFER_BINDINGS", 
GL_MAX_UNIFORM_BLOCK_SIZE, "GL_MAX_UNIFORM_BLOCK_SIZE", 
GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, "GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS", 
GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, "GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS", 
GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, "GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT", 
GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, "GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH", 
GL_ACTIVE_UNIFORM_BLOCKS, "GL_ACTIVE_UNIFORM_BLOCKS", 
GL_UNIFORM_TYPE, "GL_UNIFORM_TYPE", 
GL_UNIFORM_SIZE, "GL_UNIFORM_SIZE", 
GL_UNIFORM_NAME_LENGTH, "GL_UNIFORM_NAME_LENGTH", 
GL_UNIFORM_BLOCK_INDEX, "GL_UNIFORM_BLOCK_INDEX", 
GL_UNIFORM_OFFSET, "GL_UNIFORM_OFFSET", 
GL_UNIFORM_ARRAY_STRIDE, "GL_UNIFORM_ARRAY_STRIDE", 
GL_UNIFORM_MATRIX_STRIDE, "GL_UNIFORM_MATRIX_STRIDE", 
GL_UNIFORM_IS_ROW_MAJOR, "GL_UNIFORM_IS_ROW_MAJOR", 
GL_UNIFORM_BLOCK_BINDING, "GL_UNIFORM_BLOCK_BINDING", 
GL_UNIFORM_BLOCK_DATA_SIZE, "GL_UNIFORM_BLOCK_DATA_SIZE", 
GL_UNIFORM_BLOCK_NAME_LENGTH, "GL_UNIFORM_BLOCK_NAME_LENGTH", 
GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, "GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS", 
GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, "GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES", 
GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER, "GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER", 
GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER, "GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER", 
GL_INVALID_INDEX, "GL_INVALID_INDEX", 
GL_MAX_VERTEX_OUTPUT_COMPONENTS, "GL_MAX_VERTEX_OUTPUT_COMPONENTS", 
GL_MAX_FRAGMENT_INPUT_COMPONENTS, "GL_MAX_FRAGMENT_INPUT_COMPONENTS", 
GL_MAX_SERVER_WAIT_TIMEOUT, "GL_MAX_SERVER_WAIT_TIMEOUT", 
GL_OBJECT_TYPE, "GL_OBJECT_TYPE", 
GL_SYNC_CONDITION, "GL_SYNC_CONDITION", 
GL_SYNC_STATUS, "GL_SYNC_STATUS", 
GL_SYNC_FLAGS, "GL_SYNC_FLAGS", 
GL_SYNC_FENCE, "GL_SYNC_FENCE", 
GL_SYNC_GPU_COMMANDS_COMPLETE, "GL_SYNC_GPU_COMMANDS_COMPLETE", 
GL_UNSIGNALED, "GL_UNSIGNALED", 
GL_SIGNALED, "GL_SIGNALED", 
GL_ALREADY_SIGNALED, "GL_ALREADY_SIGNALED", 
GL_TIMEOUT_EXPIRED, "GL_TIMEOUT_EXPIRED", 
GL_CONDITION_SATISFIED, "GL_CONDITION_SATISFIED", 
GL_WAIT_FAILED, "GL_WAIT_FAILED", 
GL_SYNC_FLUSH_COMMANDS_BIT, "GL_SYNC_FLUSH_COMMANDS_BIT", 
//GL_TIMEOUT_IGNORED, "GL_TIMEOUT_IGNORED", 
GL_VERTEX_ATTRIB_ARRAY_DIVISOR, "GL_VERTEX_ATTRIB_ARRAY_DIVISOR", 
GL_ANY_SAMPLES_PASSED, "GL_ANY_SAMPLES_PASSED", 
//GL_ANY_SAMPLES_PASSED_CONSERVATIVE, "GL_ANY_SAMPLES_PASSED_CONSERVATIVE", 
GL_SAMPLER_BINDING, "GL_SAMPLER_BINDING", 
GL_RGB10_A2UI, "GL_RGB10_A2UI", 
GL_TEXTURE_SWIZZLE_R, "GL_TEXTURE_SWIZZLE_R", 
GL_TEXTURE_SWIZZLE_G, "GL_TEXTURE_SWIZZLE_G", 
GL_TEXTURE_SWIZZLE_B, "GL_TEXTURE_SWIZZLE_B", 
GL_TEXTURE_SWIZZLE_A, "GL_TEXTURE_SWIZZLE_A", 
GL_GREEN, "GL_GREEN", 
GL_BLUE, "GL_BLUE", 
GL_INT_2_10_10_10_REV, "GL_INT_2_10_10_10_REV", 
GL_TRANSFORM_FEEDBACK, "GL_TRANSFORM_FEEDBACK", 
//GL_TRANSFORM_FEEDBACK_PAUSED, "GL_TRANSFORM_FEEDBACK_PAUSED", 
//GL_TRANSFORM_FEEDBACK_ACTIVE, "GL_TRANSFORM_FEEDBACK_ACTIVE", 
GL_TRANSFORM_FEEDBACK_BINDING, "GL_TRANSFORM_FEEDBACK_BINDING", 
GL_PROGRAM_BINARY_RETRIEVABLE_HINT, "GL_PROGRAM_BINARY_RETRIEVABLE_HINT", 
GL_PROGRAM_BINARY_LENGTH, "GL_PROGRAM_BINARY_LENGTH", 
GL_NUM_PROGRAM_BINARY_FORMATS, "GL_NUM_PROGRAM_BINARY_FORMATS", 
GL_PROGRAM_BINARY_FORMATS, "GL_PROGRAM_BINARY_FORMATS", 
//GL_COMPRESSED_R11_EAC, "GL_COMPRESSED_R11_EAC", 
//GL_COMPRESSED_SIGNED_R11_EAC, "GL_COMPRESSED_SIGNED_R11_EAC", 
//GL_COMPRESSED_RG11_EAC, "GL_COMPRESSED_RG11_EAC", 
//GL_COMPRESSED_SIGNED_RG11_EAC, "GL_COMPRESSED_SIGNED_RG11_EAC", 
//GL_COMPRESSED_RGB8_ETC2, "GL_COMPRESSED_RGB8_ETC2", 
//GL_COMPRESSED_SRGB8_ETC2, "GL_COMPRESSED_SRGB8_ETC2", 
//GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2, "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2", 
//GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2, "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2", 
//GL_COMPRESSED_RGBA8_ETC2_EAC, "GL_COMPRESSED_RGBA8_ETC2_EAC", 
//GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC, "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC", 
GL_TEXTURE_IMMUTABLE_FORMAT, "GL_TEXTURE_IMMUTABLE_FORMAT", 
//GL_MAX_ELEMENT_INDEX, "GL_MAX_ELEMENT_INDEX", 
GL_NUM_SAMPLE_COUNTS, "GL_NUM_SAMPLE_COUNTS", 
//GL_TEXTURE_IMMUTABLE_LEVELS, "GL_TEXTURE_IMMUTABLE_LEVELS", 