#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glad/gl.h>

#ifndef GLAD_IMPL_UTIL_C_
#define GLAD_IMPL_UTIL_C_

#ifdef _MSC_VER
#define GLAD_IMPL_UTIL_SSCANF sscanf_s
#else
#define GLAD_IMPL_UTIL_SSCANF sscanf
#endif

#endif /* GLAD_IMPL_UTIL_C_ */


int GLAD_GL_VERSION_1_0 = 0;
int GLAD_GL_VERSION_1_1 = 0;
int GLAD_GL_VERSION_1_2 = 0;
int GLAD_GL_VERSION_1_3 = 0;
int GLAD_GL_VERSION_1_4 = 0;
int GLAD_GL_VERSION_1_5 = 0;
int GLAD_GL_VERSION_2_0 = 0;
int GLAD_GL_VERSION_2_1 = 0;
int GLAD_GL_VERSION_3_0 = 0;
int GLAD_GL_VERSION_3_1 = 0;
int GLAD_GL_VERSION_3_2 = 0;



PFNGLACTIVETEXTUREPROC glad_glActiveTexture = NULL;
PFNGLATTACHSHADERPROC glad_glAttachShader = NULL;
PFNGLBEGINCONDITIONALRENDERPROC glad_glBeginConditionalRender = NULL;
PFNGLBEGINQUERYPROC glad_glBeginQuery = NULL;
PFNGLBEGINTRANSFORMFEEDBACKPROC glad_glBeginTransformFeedback = NULL;
PFNGLBINDATTRIBLOCATIONPROC glad_glBindAttribLocation = NULL;
PFNGLBINDBUFFERPROC glad_glBindBuffer = NULL;
PFNGLBINDBUFFERBASEPROC glad_glBindBufferBase = NULL;
PFNGLBINDBUFFERRANGEPROC glad_glBindBufferRange = NULL;
PFNGLBINDFRAGDATALOCATIONPROC glad_glBindFragDataLocation = NULL;
PFNGLBINDFRAMEBUFFERPROC glad_glBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFERPROC glad_glBindRenderbuffer = NULL;
PFNGLBINDTEXTUREPROC glad_glBindTexture = NULL;
PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray = NULL;
PFNGLBLENDCOLORPROC glad_glBlendColor = NULL;
PFNGLBLENDEQUATIONPROC glad_glBlendEquation = NULL;
PFNGLBLENDEQUATIONSEPARATEPROC glad_glBlendEquationSeparate = NULL;
PFNGLBLENDFUNCPROC glad_glBlendFunc = NULL;
PFNGLBLENDFUNCSEPARATEPROC glad_glBlendFuncSeparate = NULL;
PFNGLBLITFRAMEBUFFERPROC glad_glBlitFramebuffer = NULL;
PFNGLBUFFERDATAPROC glad_glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC glad_glBufferSubData = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glad_glCheckFramebufferStatus = NULL;
PFNGLCLAMPCOLORPROC glad_glClampColor = NULL;
PFNGLCLEARPROC glad_glClear = NULL;
PFNGLCLEARBUFFERFIPROC glad_glClearBufferfi = NULL;
PFNGLCLEARBUFFERFVPROC glad_glClearBufferfv = NULL;
PFNGLCLEARBUFFERIVPROC glad_glClearBufferiv = NULL;
PFNGLCLEARBUFFERUIVPROC glad_glClearBufferuiv = NULL;
PFNGLCLEARCOLORPROC glad_glClearColor = NULL;
PFNGLCLEARDEPTHPROC glad_glClearDepth = NULL;
PFNGLCLEARSTENCILPROC glad_glClearStencil = NULL;
PFNGLCLIENTWAITSYNCPROC glad_glClientWaitSync = NULL;
PFNGLCOLORMASKPROC glad_glColorMask = NULL;
PFNGLCOLORMASKIPROC glad_glColorMaski = NULL;
PFNGLCOMPILESHADERPROC glad_glCompileShader = NULL;
PFNGLCOMPRESSEDTEXIMAGE1DPROC glad_glCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glad_glCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3DPROC glad_glCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glad_glCompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glad_glCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glad_glCompressedTexSubImage3D = NULL;
PFNGLCOPYBUFFERSUBDATAPROC glad_glCopyBufferSubData = NULL;
PFNGLCOPYTEXIMAGE1DPROC glad_glCopyTexImage1D = NULL;
PFNGLCOPYTEXIMAGE2DPROC glad_glCopyTexImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE1DPROC glad_glCopyTexSubImage1D = NULL;
PFNGLCOPYTEXSUBIMAGE2DPROC glad_glCopyTexSubImage2D = NULL;
PFNGLCOPYTEXSUBIMAGE3DPROC glad_glCopyTexSubImage3D = NULL;
PFNGLCREATEPROGRAMPROC glad_glCreateProgram = NULL;
PFNGLCREATESHADERPROC glad_glCreateShader = NULL;
PFNGLCULLFACEPROC glad_glCullFace = NULL;
PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glad_glDeleteFramebuffers = NULL;
PFNGLDELETEPROGRAMPROC glad_glDeleteProgram = NULL;
PFNGLDELETEQUERIESPROC glad_glDeleteQueries = NULL;
PFNGLDELETERENDERBUFFERSPROC glad_glDeleteRenderbuffers = NULL;
PFNGLDELETESHADERPROC glad_glDeleteShader = NULL;
PFNGLDELETESYNCPROC glad_glDeleteSync = NULL;
PFNGLDELETETEXTURESPROC glad_glDeleteTextures = NULL;
PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays = NULL;
PFNGLDEPTHFUNCPROC glad_glDepthFunc = NULL;
PFNGLDEPTHMASKPROC glad_glDepthMask = NULL;
PFNGLDEPTHRANGEPROC glad_glDepthRange = NULL;
PFNGLDETACHSHADERPROC glad_glDetachShader = NULL;
PFNGLDISABLEPROC glad_glDisable = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray = NULL;
PFNGLDISABLEIPROC glad_glDisablei = NULL;
PFNGLDRAWARRAYSPROC glad_glDrawArrays = NULL;
PFNGLDRAWARRAYSINSTANCEDPROC glad_glDrawArraysInstanced = NULL;
PFNGLDRAWBUFFERPROC glad_glDrawBuffer = NULL;
PFNGLDRAWBUFFERSPROC glad_glDrawBuffers = NULL;
PFNGLDRAWELEMENTSPROC glad_glDrawElements = NULL;
PFNGLDRAWELEMENTSBASEVERTEXPROC glad_glDrawElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDPROC glad_glDrawElementsInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glad_glDrawElementsInstancedBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSPROC glad_glDrawRangeElements = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC glad_glDrawRangeElementsBaseVertex = NULL;
PFNGLENABLEPROC glad_glEnable = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray = NULL;
PFNGLENABLEIPROC glad_glEnablei = NULL;
PFNGLENDCONDITIONALRENDERPROC glad_glEndConditionalRender = NULL;
PFNGLENDQUERYPROC glad_glEndQuery = NULL;
PFNGLENDTRANSFORMFEEDBACKPROC glad_glEndTransformFeedback = NULL;
PFNGLFENCESYNCPROC glad_glFenceSync = NULL;
PFNGLFINISHPROC glad_glFinish = NULL;
PFNGLFLUSHPROC glad_glFlush = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glad_glFlushMappedBufferRange = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glad_glFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC glad_glFramebufferTexture = NULL;
PFNGLFRAMEBUFFERTEXTURE1DPROC glad_glFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glad_glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3DPROC glad_glFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glad_glFramebufferTextureLayer = NULL;
PFNGLFRONTFACEPROC glad_glFrontFace = NULL;
PFNGLGENBUFFERSPROC glad_glGenBuffers = NULL;
PFNGLGENFRAMEBUFFERSPROC glad_glGenFramebuffers = NULL;
PFNGLGENQUERIESPROC glad_glGenQueries = NULL;
PFNGLGENRENDERBUFFERSPROC glad_glGenRenderbuffers = NULL;
PFNGLGENTEXTURESPROC glad_glGenTextures = NULL;
PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays = NULL;
PFNGLGENERATEMIPMAPPROC glad_glGenerateMipmap = NULL;
PFNGLGETACTIVEATTRIBPROC glad_glGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORMPROC glad_glGetActiveUniform = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glad_glGetActiveUniformBlockName = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glad_glGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMNAMEPROC glad_glGetActiveUniformName = NULL;
PFNGLGETACTIVEUNIFORMSIVPROC glad_glGetActiveUniformsiv = NULL;
PFNGLGETATTACHEDSHADERSPROC glad_glGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATIONPROC glad_glGetAttribLocation = NULL;
PFNGLGETBOOLEANI_VPROC glad_glGetBooleani_v = NULL;
PFNGLGETBOOLEANVPROC glad_glGetBooleanv = NULL;
PFNGLGETBUFFERPARAMETERI64VPROC glad_glGetBufferParameteri64v = NULL;
PFNGLGETBUFFERPARAMETERIVPROC glad_glGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERVPROC glad_glGetBufferPointerv = NULL;
PFNGLGETBUFFERSUBDATAPROC glad_glGetBufferSubData = NULL;
PFNGLGETCOMPRESSEDTEXIMAGEPROC glad_glGetCompressedTexImage = NULL;
PFNGLGETDOUBLEVPROC glad_glGetDoublev = NULL;
PFNGLGETERRORPROC glad_glGetError = NULL;
PFNGLGETFLOATVPROC glad_glGetFloatv = NULL;
PFNGLGETFRAGDATALOCATIONPROC glad_glGetFragDataLocation = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glad_glGetFramebufferAttachmentParameteriv = NULL;
PFNGLGETINTEGER64I_VPROC glad_glGetInteger64i_v = NULL;
PFNGLGETINTEGER64VPROC glad_glGetInteger64v = NULL;
PFNGLGETINTEGERI_VPROC glad_glGetIntegeri_v = NULL;
PFNGLGETINTEGERVPROC glad_glGetIntegerv = NULL;
PFNGLGETMULTISAMPLEFVPROC glad_glGetMultisamplefv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glad_glGetProgramiv = NULL;
PFNGLGETQUERYOBJECTIVPROC glad_glGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIVPROC glad_glGetQueryObjectuiv = NULL;
PFNGLGETQUERYIVPROC glad_glGetQueryiv = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glad_glGetRenderbufferParameteriv = NULL;
PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCEPROC glad_glGetShaderSource = NULL;
PFNGLGETSHADERIVPROC glad_glGetShaderiv = NULL;
PFNGLGETSTRINGPROC glad_glGetString = NULL;
PFNGLGETSTRINGIPROC glad_glGetStringi = NULL;
PFNGLGETSYNCIVPROC glad_glGetSynciv = NULL;
PFNGLGETTEXIMAGEPROC glad_glGetTexImage = NULL;
PFNGLGETTEXLEVELPARAMETERFVPROC glad_glGetTexLevelParameterfv = NULL;
PFNGLGETTEXLEVELPARAMETERIVPROC glad_glGetTexLevelParameteriv = NULL;
PFNGLGETTEXPARAMETERIIVPROC glad_glGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIVPROC glad_glGetTexParameterIuiv = NULL;
PFNGLGETTEXPARAMETERFVPROC glad_glGetTexParameterfv = NULL;
PFNGLGETTEXPARAMETERIVPROC glad_glGetTexParameteriv = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glad_glGetTransformFeedbackVarying = NULL;
PFNGLGETUNIFORMBLOCKINDEXPROC glad_glGetUniformBlockIndex = NULL;
PFNGLGETUNIFORMINDICESPROC glad_glGetUniformIndices = NULL;
PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation = NULL;
PFNGLGETUNIFORMFVPROC glad_glGetUniformfv = NULL;
PFNGLGETUNIFORMIVPROC glad_glGetUniformiv = NULL;
PFNGLGETUNIFORMUIVPROC glad_glGetUniformuiv = NULL;
PFNGLGETVERTEXATTRIBIIVPROC glad_glGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIVPROC glad_glGetVertexAttribIuiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERVPROC glad_glGetVertexAttribPointerv = NULL;
PFNGLGETVERTEXATTRIBDVPROC glad_glGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFVPROC glad_glGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIVPROC glad_glGetVertexAttribiv = NULL;
PFNGLHINTPROC glad_glHint = NULL;
PFNGLISBUFFERPROC glad_glIsBuffer = NULL;
PFNGLISENABLEDPROC glad_glIsEnabled = NULL;
PFNGLISENABLEDIPROC glad_glIsEnabledi = NULL;
PFNGLISFRAMEBUFFERPROC glad_glIsFramebuffer = NULL;
PFNGLISPROGRAMPROC glad_glIsProgram = NULL;
PFNGLISQUERYPROC glad_glIsQuery = NULL;
PFNGLISRENDERBUFFERPROC glad_glIsRenderbuffer = NULL;
PFNGLISSHADERPROC glad_glIsShader = NULL;
PFNGLISSYNCPROC glad_glIsSync = NULL;
PFNGLISTEXTUREPROC glad_glIsTexture = NULL;
PFNGLISVERTEXARRAYPROC glad_glIsVertexArray = NULL;
PFNGLLINEWIDTHPROC glad_glLineWidth = NULL;
PFNGLLINKPROGRAMPROC glad_glLinkProgram = NULL;
PFNGLLOGICOPPROC glad_glLogicOp = NULL;
PFNGLMAPBUFFERPROC glad_glMapBuffer = NULL;
PFNGLMAPBUFFERRANGEPROC glad_glMapBufferRange = NULL;
PFNGLMULTIDRAWARRAYSPROC glad_glMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTSPROC glad_glMultiDrawElements = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC glad_glMultiDrawElementsBaseVertex = NULL;
PFNGLPIXELSTOREFPROC glad_glPixelStoref = NULL;
PFNGLPIXELSTOREIPROC glad_glPixelStorei = NULL;
PFNGLPOINTPARAMETERFPROC glad_glPointParameterf = NULL;
PFNGLPOINTPARAMETERFVPROC glad_glPointParameterfv = NULL;
PFNGLPOINTPARAMETERIPROC glad_glPointParameteri = NULL;
PFNGLPOINTPARAMETERIVPROC glad_glPointParameteriv = NULL;
PFNGLPOINTSIZEPROC glad_glPointSize = NULL;
PFNGLPOLYGONMODEPROC glad_glPolygonMode = NULL;
PFNGLPOLYGONOFFSETPROC glad_glPolygonOffset = NULL;
PFNGLPRIMITIVERESTARTINDEXPROC glad_glPrimitiveRestartIndex = NULL;
PFNGLPROVOKINGVERTEXPROC glad_glProvokingVertex = NULL;
PFNGLREADBUFFERPROC glad_glReadBuffer = NULL;
PFNGLREADPIXELSPROC glad_glReadPixels = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glad_glRenderbufferStorage = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glad_glRenderbufferStorageMultisample = NULL;
PFNGLSAMPLECOVERAGEPROC glad_glSampleCoverage = NULL;
PFNGLSAMPLEMASKIPROC glad_glSampleMaski = NULL;
PFNGLSCISSORPROC glad_glScissor = NULL;
PFNGLSHADERSOURCEPROC glad_glShaderSource = NULL;
PFNGLSTENCILFUNCPROC glad_glStencilFunc = NULL;
PFNGLSTENCILFUNCSEPARATEPROC glad_glStencilFuncSeparate = NULL;
PFNGLSTENCILMASKPROC glad_glStencilMask = NULL;
PFNGLSTENCILMASKSEPARATEPROC glad_glStencilMaskSeparate = NULL;
PFNGLSTENCILOPPROC glad_glStencilOp = NULL;
PFNGLSTENCILOPSEPARATEPROC glad_glStencilOpSeparate = NULL;
PFNGLTEXBUFFERPROC glad_glTexBuffer = NULL;
PFNGLTEXIMAGE1DPROC glad_glTexImage1D = NULL;
PFNGLTEXIMAGE2DPROC glad_glTexImage2D = NULL;
PFNGLTEXIMAGE2DMULTISAMPLEPROC glad_glTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DPROC glad_glTexImage3D = NULL;
PFNGLTEXIMAGE3DMULTISAMPLEPROC glad_glTexImage3DMultisample = NULL;
PFNGLTEXPARAMETERIIVPROC glad_glTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIVPROC glad_glTexParameterIuiv = NULL;
PFNGLTEXPARAMETERFPROC glad_glTexParameterf = NULL;
PFNGLTEXPARAMETERFVPROC glad_glTexParameterfv = NULL;
PFNGLTEXPARAMETERIPROC glad_glTexParameteri = NULL;
PFNGLTEXPARAMETERIVPROC glad_glTexParameteriv = NULL;
PFNGLTEXSUBIMAGE1DPROC glad_glTexSubImage1D = NULL;
PFNGLTEXSUBIMAGE2DPROC glad_glTexSubImage2D = NULL;
PFNGLTEXSUBIMAGE3DPROC glad_glTexSubImage3D = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glad_glTransformFeedbackVaryings = NULL;
PFNGLUNIFORM1FPROC glad_glUniform1f = NULL;
PFNGLUNIFORM1FVPROC glad_glUniform1fv = NULL;
PFNGLUNIFORM1IPROC glad_glUniform1i = NULL;
PFNGLUNIFORM1IVPROC glad_glUniform1iv = NULL;
PFNGLUNIFORM1UIPROC glad_glUniform1ui = NULL;
PFNGLUNIFORM1UIVPROC glad_glUniform1uiv = NULL;
PFNGLUNIFORM2FPROC glad_glUniform2f = NULL;
PFNGLUNIFORM2FVPROC glad_glUniform2fv = NULL;
PFNGLUNIFORM2IPROC glad_glUniform2i = NULL;
PFNGLUNIFORM2IVPROC glad_glUniform2iv = NULL;
PFNGLUNIFORM2UIPROC glad_glUniform2ui = NULL;
PFNGLUNIFORM2UIVPROC glad_glUniform2uiv = NULL;
PFNGLUNIFORM3FPROC glad_glUniform3f = NULL;
PFNGLUNIFORM3FVPROC glad_glUniform3fv = NULL;
PFNGLUNIFORM3IPROC glad_glUniform3i = NULL;
PFNGLUNIFORM3IVPROC glad_glUniform3iv = NULL;
PFNGLUNIFORM3UIPROC glad_glUniform3ui = NULL;
PFNGLUNIFORM3UIVPROC glad_glUniform3uiv = NULL;
PFNGLUNIFORM4FPROC glad_glUniform4f = NULL;
PFNGLUNIFORM4FVPROC glad_glUniform4fv = NULL;
PFNGLUNIFORM4IPROC glad_glUniform4i = NULL;
PFNGLUNIFORM4IVPROC glad_glUniform4iv = NULL;
PFNGLUNIFORM4UIPROC glad_glUniform4ui = NULL;
PFNGLUNIFORM4UIVPROC glad_glUniform4uiv = NULL;
PFNGLUNIFORMBLOCKBINDINGPROC glad_glUniformBlockBinding = NULL;
PFNGLUNIFORMMATRIX2FVPROC glad_glUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX2X3FVPROC glad_glUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX2X4FVPROC glad_glUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX3FVPROC glad_glUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX3X2FVPROC glad_glUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FVPROC glad_glUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv = NULL;
PFNGLUNIFORMMATRIX4X2FVPROC glad_glUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX4X3FVPROC glad_glUniformMatrix4x3fv = NULL;
PFNGLUNMAPBUFFERPROC glad_glUnmapBuffer = NULL;
PFNGLUSEPROGRAMPROC glad_glUseProgram = NULL;
PFNGLVALIDATEPROGRAMPROC glad_glValidateProgram = NULL;
PFNGLVERTEXATTRIB1DPROC glad_glVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DVPROC glad_glVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1FPROC glad_glVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FVPROC glad_glVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1SPROC glad_glVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SVPROC glad_glVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2DPROC glad_glVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DVPROC glad_glVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2FPROC glad_glVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FVPROC glad_glVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2SPROC glad_glVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SVPROC glad_glVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3DPROC glad_glVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DVPROC glad_glVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3FPROC glad_glVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FVPROC glad_glVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3SPROC glad_glVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SVPROC glad_glVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBVPROC glad_glVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIVPROC glad_glVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSVPROC glad_glVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUBPROC glad_glVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBVPROC glad_glVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIVPROC glad_glVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSVPROC glad_glVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BVPROC glad_glVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4DPROC glad_glVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DVPROC glad_glVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4FPROC glad_glVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FVPROC glad_glVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IVPROC glad_glVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4SPROC glad_glVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SVPROC glad_glVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBVPROC glad_glVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIVPROC glad_glVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USVPROC glad_glVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBI1IPROC glad_glVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI1IVPROC glad_glVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI1UIPROC glad_glVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI1UIVPROC glad_glVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2IPROC glad_glVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI2IVPROC glad_glVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI2UIPROC glad_glVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI2UIVPROC glad_glVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3IPROC glad_glVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI3IVPROC glad_glVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI3UIPROC glad_glVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI3UIVPROC glad_glVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4BVPROC glad_glVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4IPROC glad_glVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI4IVPROC glad_glVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI4SVPROC glad_glVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBVPROC glad_glVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4UIPROC glad_glVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI4UIVPROC glad_glVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4USVPROC glad_glVertexAttribI4usv = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glad_glVertexAttribIPointer = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer = NULL;
PFNGLVIEWPORTPROC glad_glViewport = NULL;
PFNGLWAITSYNCPROC glad_glWaitSync = NULL;


static void glad_gl_load_GL_VERSION_1_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_0) return;
    glBlendFunc = (PFNGLBLENDFUNCPROC) load("glBlendFunc", userptr);
    glClear = (PFNGLCLEARPROC) load("glClear", userptr);
    glClearColor = (PFNGLCLEARCOLORPROC) load("glClearColor", userptr);
    glClearDepth = (PFNGLCLEARDEPTHPROC) load("glClearDepth", userptr);
    glClearStencil = (PFNGLCLEARSTENCILPROC) load("glClearStencil", userptr);
    glColorMask = (PFNGLCOLORMASKPROC) load("glColorMask", userptr);
    glCullFace = (PFNGLCULLFACEPROC) load("glCullFace", userptr);
    glDepthFunc = (PFNGLDEPTHFUNCPROC) load("glDepthFunc", userptr);
    glDepthMask = (PFNGLDEPTHMASKPROC) load("glDepthMask", userptr);
    glDepthRange = (PFNGLDEPTHRANGEPROC) load("glDepthRange", userptr);
    glDisable = (PFNGLDISABLEPROC) load("glDisable", userptr);
    glDrawBuffer = (PFNGLDRAWBUFFERPROC) load("glDrawBuffer", userptr);
    glEnable = (PFNGLENABLEPROC) load("glEnable", userptr);
    glFinish = (PFNGLFINISHPROC) load("glFinish", userptr);
    glFlush = (PFNGLFLUSHPROC) load("glFlush", userptr);
    glFrontFace = (PFNGLFRONTFACEPROC) load("glFrontFace", userptr);
    glGetBooleanv = (PFNGLGETBOOLEANVPROC) load("glGetBooleanv", userptr);
    glGetDoublev = (PFNGLGETDOUBLEVPROC) load("glGetDoublev", userptr);
    glGetError = (PFNGLGETERRORPROC) load("glGetError", userptr);
    glGetFloatv = (PFNGLGETFLOATVPROC) load("glGetFloatv", userptr);
    glGetIntegerv = (PFNGLGETINTEGERVPROC) load("glGetIntegerv", userptr);
    glGetString = (PFNGLGETSTRINGPROC) load("glGetString", userptr);
    glGetTexImage = (PFNGLGETTEXIMAGEPROC) load("glGetTexImage", userptr);
    glGetTexLevelParameterfv = (PFNGLGETTEXLEVELPARAMETERFVPROC) load("glGetTexLevelParameterfv", userptr);
    glGetTexLevelParameteriv = (PFNGLGETTEXLEVELPARAMETERIVPROC) load("glGetTexLevelParameteriv", userptr);
    glGetTexParameterfv = (PFNGLGETTEXPARAMETERFVPROC) load("glGetTexParameterfv", userptr);
    glGetTexParameteriv = (PFNGLGETTEXPARAMETERIVPROC) load("glGetTexParameteriv", userptr);
    glHint = (PFNGLHINTPROC) load("glHint", userptr);
    glIsEnabled = (PFNGLISENABLEDPROC) load("glIsEnabled", userptr);
    glLineWidth = (PFNGLLINEWIDTHPROC) load("glLineWidth", userptr);
    glLogicOp = (PFNGLLOGICOPPROC) load("glLogicOp", userptr);
    glPixelStoref = (PFNGLPIXELSTOREFPROC) load("glPixelStoref", userptr);
    glPixelStorei = (PFNGLPIXELSTOREIPROC) load("glPixelStorei", userptr);
    glPointSize = (PFNGLPOINTSIZEPROC) load("glPointSize", userptr);
    glPolygonMode = (PFNGLPOLYGONMODEPROC) load("glPolygonMode", userptr);
    glReadBuffer = (PFNGLREADBUFFERPROC) load("glReadBuffer", userptr);
    glReadPixels = (PFNGLREADPIXELSPROC) load("glReadPixels", userptr);
    glScissor = (PFNGLSCISSORPROC) load("glScissor", userptr);
    glStencilFunc = (PFNGLSTENCILFUNCPROC) load("glStencilFunc", userptr);
    glStencilMask = (PFNGLSTENCILMASKPROC) load("glStencilMask", userptr);
    glStencilOp = (PFNGLSTENCILOPPROC) load("glStencilOp", userptr);
    glTexImage1D = (PFNGLTEXIMAGE1DPROC) load("glTexImage1D", userptr);
    glTexImage2D = (PFNGLTEXIMAGE2DPROC) load("glTexImage2D", userptr);
    glTexParameterf = (PFNGLTEXPARAMETERFPROC) load("glTexParameterf", userptr);
    glTexParameterfv = (PFNGLTEXPARAMETERFVPROC) load("glTexParameterfv", userptr);
    glTexParameteri = (PFNGLTEXPARAMETERIPROC) load("glTexParameteri", userptr);
    glTexParameteriv = (PFNGLTEXPARAMETERIVPROC) load("glTexParameteriv", userptr);
    glViewport = (PFNGLVIEWPORTPROC) load("glViewport", userptr);
}
static void glad_gl_load_GL_VERSION_1_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_1) return;
    glBindTexture = (PFNGLBINDTEXTUREPROC) load("glBindTexture", userptr);
    glCopyTexImage1D = (PFNGLCOPYTEXIMAGE1DPROC) load("glCopyTexImage1D", userptr);
    glCopyTexImage2D = (PFNGLCOPYTEXIMAGE2DPROC) load("glCopyTexImage2D", userptr);
    glCopyTexSubImage1D = (PFNGLCOPYTEXSUBIMAGE1DPROC) load("glCopyTexSubImage1D", userptr);
    glCopyTexSubImage2D = (PFNGLCOPYTEXSUBIMAGE2DPROC) load("glCopyTexSubImage2D", userptr);
    glDeleteTextures = (PFNGLDELETETEXTURESPROC) load("glDeleteTextures", userptr);
    glDrawArrays = (PFNGLDRAWARRAYSPROC) load("glDrawArrays", userptr);
    glDrawElements = (PFNGLDRAWELEMENTSPROC) load("glDrawElements", userptr);
    glGenTextures = (PFNGLGENTEXTURESPROC) load("glGenTextures", userptr);
    glIsTexture = (PFNGLISTEXTUREPROC) load("glIsTexture", userptr);
    glPolygonOffset = (PFNGLPOLYGONOFFSETPROC) load("glPolygonOffset", userptr);
    glTexSubImage1D = (PFNGLTEXSUBIMAGE1DPROC) load("glTexSubImage1D", userptr);
    glTexSubImage2D = (PFNGLTEXSUBIMAGE2DPROC) load("glTexSubImage2D", userptr);
}
static void glad_gl_load_GL_VERSION_1_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_2) return;
    glCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3DPROC) load("glCopyTexSubImage3D", userptr);
    glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) load("glDrawRangeElements", userptr);
    glTexImage3D = (PFNGLTEXIMAGE3DPROC) load("glTexImage3D", userptr);
    glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC) load("glTexSubImage3D", userptr);
}
static void glad_gl_load_GL_VERSION_1_3( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_3) return;
    glActiveTexture = (PFNGLACTIVETEXTUREPROC) load("glActiveTexture", userptr);
    glCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1DPROC) load("glCompressedTexImage1D", userptr);
    glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC) load("glCompressedTexImage2D", userptr);
    glCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3DPROC) load("glCompressedTexImage3D", userptr);
    glCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) load("glCompressedTexSubImage1D", userptr);
    glCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) load("glCompressedTexSubImage2D", userptr);
    glCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) load("glCompressedTexSubImage3D", userptr);
    glGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGEPROC) load("glGetCompressedTexImage", userptr);
    glSampleCoverage = (PFNGLSAMPLECOVERAGEPROC) load("glSampleCoverage", userptr);
}
static void glad_gl_load_GL_VERSION_1_4( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_4) return;
    glBlendColor = (PFNGLBLENDCOLORPROC) load("glBlendColor", userptr);
    glBlendEquation = (PFNGLBLENDEQUATIONPROC) load("glBlendEquation", userptr);
    glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) load("glBlendFuncSeparate", userptr);
    glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC) load("glMultiDrawArrays", userptr);
    glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC) load("glMultiDrawElements", userptr);
    glPointParameterf = (PFNGLPOINTPARAMETERFPROC) load("glPointParameterf", userptr);
    glPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) load("glPointParameterfv", userptr);
    glPointParameteri = (PFNGLPOINTPARAMETERIPROC) load("glPointParameteri", userptr);
    glPointParameteriv = (PFNGLPOINTPARAMETERIVPROC) load("glPointParameteriv", userptr);
}
static void glad_gl_load_GL_VERSION_1_5( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_1_5) return;
    glBeginQuery = (PFNGLBEGINQUERYPROC) load("glBeginQuery", userptr);
    glBindBuffer = (PFNGLBINDBUFFERPROC) load("glBindBuffer", userptr);
    glBufferData = (PFNGLBUFFERDATAPROC) load("glBufferData", userptr);
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC) load("glBufferSubData", userptr);
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) load("glDeleteBuffers", userptr);
    glDeleteQueries = (PFNGLDELETEQUERIESPROC) load("glDeleteQueries", userptr);
    glEndQuery = (PFNGLENDQUERYPROC) load("glEndQuery", userptr);
    glGenBuffers = (PFNGLGENBUFFERSPROC) load("glGenBuffers", userptr);
    glGenQueries = (PFNGLGENQUERIESPROC) load("glGenQueries", userptr);
    glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC) load("glGetBufferParameteriv", userptr);
    glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC) load("glGetBufferPointerv", userptr);
    glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC) load("glGetBufferSubData", userptr);
    glGetQueryObjectiv = (PFNGLGETQUERYOBJECTIVPROC) load("glGetQueryObjectiv", userptr);
    glGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIVPROC) load("glGetQueryObjectuiv", userptr);
    glGetQueryiv = (PFNGLGETQUERYIVPROC) load("glGetQueryiv", userptr);
    glIsBuffer = (PFNGLISBUFFERPROC) load("glIsBuffer", userptr);
    glIsQuery = (PFNGLISQUERYPROC) load("glIsQuery", userptr);
    glMapBuffer = (PFNGLMAPBUFFERPROC) load("glMapBuffer", userptr);
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) load("glUnmapBuffer", userptr);
}
static void glad_gl_load_GL_VERSION_2_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_2_0) return;
    glAttachShader = (PFNGLATTACHSHADERPROC) load("glAttachShader", userptr);
    glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC) load("glBindAttribLocation", userptr);
    glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC) load("glBlendEquationSeparate", userptr);
    glCompileShader = (PFNGLCOMPILESHADERPROC) load("glCompileShader", userptr);
    glCreateProgram = (PFNGLCREATEPROGRAMPROC) load("glCreateProgram", userptr);
    glCreateShader = (PFNGLCREATESHADERPROC) load("glCreateShader", userptr);
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC) load("glDeleteProgram", userptr);
    glDeleteShader = (PFNGLDELETESHADERPROC) load("glDeleteShader", userptr);
    glDetachShader = (PFNGLDETACHSHADERPROC) load("glDetachShader", userptr);
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) load("glDisableVertexAttribArray", userptr);
    glDrawBuffers = (PFNGLDRAWBUFFERSPROC) load("glDrawBuffers", userptr);
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) load("glEnableVertexAttribArray", userptr);
    glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) load("glGetActiveAttrib", userptr);
    glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC) load("glGetActiveUniform", userptr);
    glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC) load("glGetAttachedShaders", userptr);
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) load("glGetAttribLocation", userptr);
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) load("glGetProgramInfoLog", userptr);
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC) load("glGetProgramiv", userptr);
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) load("glGetShaderInfoLog", userptr);
    glGetShaderSource = (PFNGLGETSHADERSOURCEPROC) load("glGetShaderSource", userptr);
    glGetShaderiv = (PFNGLGETSHADERIVPROC) load("glGetShaderiv", userptr);
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) load("glGetUniformLocation", userptr);
    glGetUniformfv = (PFNGLGETUNIFORMFVPROC) load("glGetUniformfv", userptr);
    glGetUniformiv = (PFNGLGETUNIFORMIVPROC) load("glGetUniformiv", userptr);
    glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC) load("glGetVertexAttribPointerv", userptr);
    glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC) load("glGetVertexAttribdv", userptr);
    glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC) load("glGetVertexAttribfv", userptr);
    glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC) load("glGetVertexAttribiv", userptr);
    glIsProgram = (PFNGLISPROGRAMPROC) load("glIsProgram", userptr);
    glIsShader = (PFNGLISSHADERPROC) load("glIsShader", userptr);
    glLinkProgram = (PFNGLLINKPROGRAMPROC) load("glLinkProgram", userptr);
    glShaderSource = (PFNGLSHADERSOURCEPROC) load("glShaderSource", userptr);
    glStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATEPROC) load("glStencilFuncSeparate", userptr);
    glStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATEPROC) load("glStencilMaskSeparate", userptr);
    glStencilOpSeparate = (PFNGLSTENCILOPSEPARATEPROC) load("glStencilOpSeparate", userptr);
    glUniform1f = (PFNGLUNIFORM1FPROC) load("glUniform1f", userptr);
    glUniform1fv = (PFNGLUNIFORM1FVPROC) load("glUniform1fv", userptr);
    glUniform1i = (PFNGLUNIFORM1IPROC) load("glUniform1i", userptr);
    glUniform1iv = (PFNGLUNIFORM1IVPROC) load("glUniform1iv", userptr);
    glUniform2f = (PFNGLUNIFORM2FPROC) load("glUniform2f", userptr);
    glUniform2fv = (PFNGLUNIFORM2FVPROC) load("glUniform2fv", userptr);
    glUniform2i = (PFNGLUNIFORM2IPROC) load("glUniform2i", userptr);
    glUniform2iv = (PFNGLUNIFORM2IVPROC) load("glUniform2iv", userptr);
    glUniform3f = (PFNGLUNIFORM3FPROC) load("glUniform3f", userptr);
    glUniform3fv = (PFNGLUNIFORM3FVPROC) load("glUniform3fv", userptr);
    glUniform3i = (PFNGLUNIFORM3IPROC) load("glUniform3i", userptr);
    glUniform3iv = (PFNGLUNIFORM3IVPROC) load("glUniform3iv", userptr);
    glUniform4f = (PFNGLUNIFORM4FPROC) load("glUniform4f", userptr);
    glUniform4fv = (PFNGLUNIFORM4FVPROC) load("glUniform4fv", userptr);
    glUniform4i = (PFNGLUNIFORM4IPROC) load("glUniform4i", userptr);
    glUniform4iv = (PFNGLUNIFORM4IVPROC) load("glUniform4iv", userptr);
    glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC) load("glUniformMatrix2fv", userptr);
    glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC) load("glUniformMatrix3fv", userptr);
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) load("glUniformMatrix4fv", userptr);
    glUseProgram = (PFNGLUSEPROGRAMPROC) load("glUseProgram", userptr);
    glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) load("glValidateProgram", userptr);
    glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC) load("glVertexAttrib1d", userptr);
    glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC) load("glVertexAttrib1dv", userptr);
    glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC) load("glVertexAttrib1f", userptr);
    glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC) load("glVertexAttrib1fv", userptr);
    glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC) load("glVertexAttrib1s", userptr);
    glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC) load("glVertexAttrib1sv", userptr);
    glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC) load("glVertexAttrib2d", userptr);
    glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC) load("glVertexAttrib2dv", userptr);
    glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC) load("glVertexAttrib2f", userptr);
    glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC) load("glVertexAttrib2fv", userptr);
    glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC) load("glVertexAttrib2s", userptr);
    glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC) load("glVertexAttrib2sv", userptr);
    glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC) load("glVertexAttrib3d", userptr);
    glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC) load("glVertexAttrib3dv", userptr);
    glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC) load("glVertexAttrib3f", userptr);
    glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC) load("glVertexAttrib3fv", userptr);
    glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC) load("glVertexAttrib3s", userptr);
    glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC) load("glVertexAttrib3sv", userptr);
    glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC) load("glVertexAttrib4Nbv", userptr);
    glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC) load("glVertexAttrib4Niv", userptr);
    glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC) load("glVertexAttrib4Nsv", userptr);
    glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC) load("glVertexAttrib4Nub", userptr);
    glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC) load("glVertexAttrib4Nubv", userptr);
    glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC) load("glVertexAttrib4Nuiv", userptr);
    glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC) load("glVertexAttrib4Nusv", userptr);
    glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC) load("glVertexAttrib4bv", userptr);
    glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC) load("glVertexAttrib4d", userptr);
    glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC) load("glVertexAttrib4dv", userptr);
    glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) load("glVertexAttrib4f", userptr);
    glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC) load("glVertexAttrib4fv", userptr);
    glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC) load("glVertexAttrib4iv", userptr);
    glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC) load("glVertexAttrib4s", userptr);
    glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC) load("glVertexAttrib4sv", userptr);
    glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC) load("glVertexAttrib4ubv", userptr);
    glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC) load("glVertexAttrib4uiv", userptr);
    glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC) load("glVertexAttrib4usv", userptr);
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) load("glVertexAttribPointer", userptr);
}
static void glad_gl_load_GL_VERSION_2_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_2_1) return;
    glUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FVPROC) load("glUniformMatrix2x3fv", userptr);
    glUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FVPROC) load("glUniformMatrix2x4fv", userptr);
    glUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FVPROC) load("glUniformMatrix3x2fv", userptr);
    glUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FVPROC) load("glUniformMatrix3x4fv", userptr);
    glUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FVPROC) load("glUniformMatrix4x2fv", userptr);
    glUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FVPROC) load("glUniformMatrix4x3fv", userptr);
}
static void glad_gl_load_GL_VERSION_3_0( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_0) return;
    glBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC) load("glBeginConditionalRender", userptr);
    glBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC) load("glBeginTransformFeedback", userptr);
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load("glBindBufferBase", userptr);
    glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load("glBindBufferRange", userptr);
    glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC) load("glBindFragDataLocation", userptr);
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) load("glBindFramebuffer", userptr);
    glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC) load("glBindRenderbuffer", userptr);
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) load("glBindVertexArray", userptr);
    glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) load("glBlitFramebuffer", userptr);
    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) load("glCheckFramebufferStatus", userptr);
    glClampColor = (PFNGLCLAMPCOLORPROC) load("glClampColor", userptr);
    glClearBufferfi = (PFNGLCLEARBUFFERFIPROC) load("glClearBufferfi", userptr);
    glClearBufferfv = (PFNGLCLEARBUFFERFVPROC) load("glClearBufferfv", userptr);
    glClearBufferiv = (PFNGLCLEARBUFFERIVPROC) load("glClearBufferiv", userptr);
    glClearBufferuiv = (PFNGLCLEARBUFFERUIVPROC) load("glClearBufferuiv", userptr);
    glColorMaski = (PFNGLCOLORMASKIPROC) load("glColorMaski", userptr);
    glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC) load("glDeleteFramebuffers", userptr);
    glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC) load("glDeleteRenderbuffers", userptr);
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) load("glDeleteVertexArrays", userptr);
    glDisablei = (PFNGLDISABLEIPROC) load("glDisablei", userptr);
    glEnablei = (PFNGLENABLEIPROC) load("glEnablei", userptr);
    glEndConditionalRender = (PFNGLENDCONDITIONALRENDERPROC) load("glEndConditionalRender", userptr);
    glEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACKPROC) load("glEndTransformFeedback", userptr);
    glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC) load("glFlushMappedBufferRange", userptr);
    glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC) load("glFramebufferRenderbuffer", userptr);
    glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC) load("glFramebufferTexture1D", userptr);
    glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) load("glFramebufferTexture2D", userptr);
    glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC) load("glFramebufferTexture3D", userptr);
    glFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYERPROC) load("glFramebufferTextureLayer", userptr);
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) load("glGenFramebuffers", userptr);
    glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC) load("glGenRenderbuffers", userptr);
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) load("glGenVertexArrays", userptr);
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) load("glGenerateMipmap", userptr);
    glGetBooleani_v = (PFNGLGETBOOLEANI_VPROC) load("glGetBooleani_v", userptr);
    glGetFragDataLocation = (PFNGLGETFRAGDATALOCATIONPROC) load("glGetFragDataLocation", userptr);
    glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) load("glGetFramebufferAttachmentParameteriv", userptr);
    glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load("glGetIntegeri_v", userptr);
    glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC) load("glGetRenderbufferParameteriv", userptr);
    glGetStringi = (PFNGLGETSTRINGIPROC) load("glGetStringi", userptr);
    glGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIVPROC) load("glGetTexParameterIiv", userptr);
    glGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIVPROC) load("glGetTexParameterIuiv", userptr);
    glGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) load("glGetTransformFeedbackVarying", userptr);
    glGetUniformuiv = (PFNGLGETUNIFORMUIVPROC) load("glGetUniformuiv", userptr);
    glGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIVPROC) load("glGetVertexAttribIiv", userptr);
    glGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIVPROC) load("glGetVertexAttribIuiv", userptr);
    glIsEnabledi = (PFNGLISENABLEDIPROC) load("glIsEnabledi", userptr);
    glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC) load("glIsFramebuffer", userptr);
    glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC) load("glIsRenderbuffer", userptr);
    glIsVertexArray = (PFNGLISVERTEXARRAYPROC) load("glIsVertexArray", userptr);
    glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC) load("glMapBufferRange", userptr);
    glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC) load("glRenderbufferStorage", userptr);
    glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) load("glRenderbufferStorageMultisample", userptr);
    glTexParameterIiv = (PFNGLTEXPARAMETERIIVPROC) load("glTexParameterIiv", userptr);
    glTexParameterIuiv = (PFNGLTEXPARAMETERIUIVPROC) load("glTexParameterIuiv", userptr);
    glTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGSPROC) load("glTransformFeedbackVaryings", userptr);
    glUniform1ui = (PFNGLUNIFORM1UIPROC) load("glUniform1ui", userptr);
    glUniform1uiv = (PFNGLUNIFORM1UIVPROC) load("glUniform1uiv", userptr);
    glUniform2ui = (PFNGLUNIFORM2UIPROC) load("glUniform2ui", userptr);
    glUniform2uiv = (PFNGLUNIFORM2UIVPROC) load("glUniform2uiv", userptr);
    glUniform3ui = (PFNGLUNIFORM3UIPROC) load("glUniform3ui", userptr);
    glUniform3uiv = (PFNGLUNIFORM3UIVPROC) load("glUniform3uiv", userptr);
    glUniform4ui = (PFNGLUNIFORM4UIPROC) load("glUniform4ui", userptr);
    glUniform4uiv = (PFNGLUNIFORM4UIVPROC) load("glUniform4uiv", userptr);
    glVertexAttribI1i = (PFNGLVERTEXATTRIBI1IPROC) load("glVertexAttribI1i", userptr);
    glVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IVPROC) load("glVertexAttribI1iv", userptr);
    glVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UIPROC) load("glVertexAttribI1ui", userptr);
    glVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIVPROC) load("glVertexAttribI1uiv", userptr);
    glVertexAttribI2i = (PFNGLVERTEXATTRIBI2IPROC) load("glVertexAttribI2i", userptr);
    glVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IVPROC) load("glVertexAttribI2iv", userptr);
    glVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UIPROC) load("glVertexAttribI2ui", userptr);
    glVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIVPROC) load("glVertexAttribI2uiv", userptr);
    glVertexAttribI3i = (PFNGLVERTEXATTRIBI3IPROC) load("glVertexAttribI3i", userptr);
    glVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IVPROC) load("glVertexAttribI3iv", userptr);
    glVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UIPROC) load("glVertexAttribI3ui", userptr);
    glVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIVPROC) load("glVertexAttribI3uiv", userptr);
    glVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BVPROC) load("glVertexAttribI4bv", userptr);
    glVertexAttribI4i = (PFNGLVERTEXATTRIBI4IPROC) load("glVertexAttribI4i", userptr);
    glVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IVPROC) load("glVertexAttribI4iv", userptr);
    glVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SVPROC) load("glVertexAttribI4sv", userptr);
    glVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBVPROC) load("glVertexAttribI4ubv", userptr);
    glVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UIPROC) load("glVertexAttribI4ui", userptr);
    glVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIVPROC) load("glVertexAttribI4uiv", userptr);
    glVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USVPROC) load("glVertexAttribI4usv", userptr);
    glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC) load("glVertexAttribIPointer", userptr);
}
static void glad_gl_load_GL_VERSION_3_1( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_1) return;
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC) load("glBindBufferBase", userptr);
    glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC) load("glBindBufferRange", userptr);
    glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC) load("glCopyBufferSubData", userptr);
    glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC) load("glDrawArraysInstanced", userptr);
    glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC) load("glDrawElementsInstanced", userptr);
    glGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) load("glGetActiveUniformBlockName", userptr);
    glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC) load("glGetActiveUniformBlockiv", userptr);
    glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC) load("glGetActiveUniformName", userptr);
    glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC) load("glGetActiveUniformsiv", userptr);
    glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC) load("glGetIntegeri_v", userptr);
    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC) load("glGetUniformBlockIndex", userptr);
    glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC) load("glGetUniformIndices", userptr);
    glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC) load("glPrimitiveRestartIndex", userptr);
    glTexBuffer = (PFNGLTEXBUFFERPROC) load("glTexBuffer", userptr);
    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC) load("glUniformBlockBinding", userptr);
}
static void glad_gl_load_GL_VERSION_3_2( GLADuserptrloadfunc load, void* userptr) {
    if(!GLAD_GL_VERSION_3_2) return;
    glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC) load("glClientWaitSync", userptr);
    glDeleteSync = (PFNGLDELETESYNCPROC) load("glDeleteSync", userptr);
    glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC) load("glDrawElementsBaseVertex", userptr);
    glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) load("glDrawElementsInstancedBaseVertex", userptr);
    glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) load("glDrawRangeElementsBaseVertex", userptr);
    glFenceSync = (PFNGLFENCESYNCPROC) load("glFenceSync", userptr);
    glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) load("glFramebufferTexture", userptr);
    glGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64VPROC) load("glGetBufferParameteri64v", userptr);
    glGetInteger64i_v = (PFNGLGETINTEGER64I_VPROC) load("glGetInteger64i_v", userptr);
    glGetInteger64v = (PFNGLGETINTEGER64VPROC) load("glGetInteger64v", userptr);
    glGetMultisamplefv = (PFNGLGETMULTISAMPLEFVPROC) load("glGetMultisamplefv", userptr);
    glGetSynciv = (PFNGLGETSYNCIVPROC) load("glGetSynciv", userptr);
    glIsSync = (PFNGLISSYNCPROC) load("glIsSync", userptr);
    glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) load("glMultiDrawElementsBaseVertex", userptr);
    glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC) load("glProvokingVertex", userptr);
    glSampleMaski = (PFNGLSAMPLEMASKIPROC) load("glSampleMaski", userptr);
    glTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLEPROC) load("glTexImage2DMultisample", userptr);
    glTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLEPROC) load("glTexImage3DMultisample", userptr);
    glWaitSync = (PFNGLWAITSYNCPROC) load("glWaitSync", userptr);
}



#if defined(GL_ES_VERSION_3_0) || defined(GL_VERSION_3_0)
#define GLAD_GL_IS_SOME_NEW_VERSION 1
#else
#define GLAD_GL_IS_SOME_NEW_VERSION 0
#endif

static int glad_gl_get_extensions( int version, const char **out_exts, unsigned int *out_num_exts_i, char ***out_exts_i) {
#if GLAD_GL_IS_SOME_NEW_VERSION
    if(GLAD_VERSION_MAJOR(version) < 3) {
#else
    (void) version;
    (void) out_num_exts_i;
    (void) out_exts_i;
#endif
        if (glGetString == NULL) {
            return 0;
        }
        *out_exts = (const char *)glGetString(GL_EXTENSIONS);
#if GLAD_GL_IS_SOME_NEW_VERSION
    } else {
        unsigned int index = 0;
        unsigned int num_exts_i = 0;
        char **exts_i = NULL;
        if (glGetStringi == NULL || glGetIntegerv == NULL) {
            return 0;
        }
        glGetIntegerv(GL_NUM_EXTENSIONS, (int*) &num_exts_i);
        if (num_exts_i > 0) {
            exts_i = (char **) malloc(num_exts_i * (sizeof *exts_i));
        }
        if (exts_i == NULL) {
            return 0;
        }
        for(index = 0; index < num_exts_i; index++) {
            const char *gl_str_tmp = (const char*) glGetStringi(GL_EXTENSIONS, index);
            size_t len = strlen(gl_str_tmp) + 1;

            char *local_str = (char*) malloc(len * sizeof(char));
            if(local_str != NULL) {
                memcpy(local_str, gl_str_tmp, len * sizeof(char));
            }

            exts_i[index] = local_str;
        }

        *out_num_exts_i = num_exts_i;
        *out_exts_i = exts_i;
    }
#endif
    return 1;
}
static void glad_gl_free_extensions(char **exts_i, unsigned int num_exts_i) {
    if (exts_i != NULL) {
        unsigned int index;
        for(index = 0; index < num_exts_i; index++) {
            free((void *) (exts_i[index]));
        }
        free((void *)exts_i);
        exts_i = NULL;
    }
}
static int glad_gl_has_extension(int version, const char *exts, unsigned int num_exts_i, char **exts_i, const char *ext) {
    if(GLAD_VERSION_MAJOR(version) < 3 || !GLAD_GL_IS_SOME_NEW_VERSION) {
        const char *extensions;
        const char *loc;
        const char *terminator;
        extensions = exts;
        if(extensions == NULL || ext == NULL) {
            return 0;
        }
        while(1) {
            loc = strstr(extensions, ext);
            if(loc == NULL) {
                return 0;
            }
            terminator = loc + strlen(ext);
            if((loc == extensions || *(loc - 1) == ' ') &&
                (*terminator == ' ' || *terminator == '\0')) {
                return 1;
            }
            extensions = terminator;
        }
    } else {
        unsigned int index;
        for(index = 0; index < num_exts_i; index++) {
            const char *e = exts_i[index];
            if(strcmp(e, ext) == 0) {
                return 1;
            }
        }
    }
    return 0;
}

static GLADapiproc glad_gl_get_proc_from_userptr(const char* name, void *userptr) {
    return (GLAD_GNUC_EXTENSION (GLADapiproc (*)(const char *name)) userptr)(name);
}

static int glad_gl_find_extensions_gl( int version) {
    const char *exts = NULL;
    unsigned int num_exts_i = 0;
    char **exts_i = NULL;
    if (!glad_gl_get_extensions(version, &exts, &num_exts_i, &exts_i)) return 0;

    (void) glad_gl_has_extension;

    glad_gl_free_extensions(exts_i, num_exts_i);

    return 1;
}

static int glad_gl_find_core_gl(void) {
    int i, major, minor;
    const char* version;
    const char* prefixes[] = {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES ",
        NULL
    };
    version = (const char*) glGetString(GL_VERSION);
    if (!version) return 0;
    for (i = 0;  prefixes[i];  i++) {
        const size_t length = strlen(prefixes[i]);
        if (strncmp(version, prefixes[i], length) == 0) {
            version += length;
            break;
        }
    }

    GLAD_IMPL_UTIL_SSCANF(version, "%d.%d", &major, &minor);

    GLAD_GL_VERSION_1_0 = (major == 1 && minor >= 0) || major > 1;
    GLAD_GL_VERSION_1_1 = (major == 1 && minor >= 1) || major > 1;
    GLAD_GL_VERSION_1_2 = (major == 1 && minor >= 2) || major > 1;
    GLAD_GL_VERSION_1_3 = (major == 1 && minor >= 3) || major > 1;
    GLAD_GL_VERSION_1_4 = (major == 1 && minor >= 4) || major > 1;
    GLAD_GL_VERSION_1_5 = (major == 1 && minor >= 5) || major > 1;
    GLAD_GL_VERSION_2_0 = (major == 2 && minor >= 0) || major > 2;
    GLAD_GL_VERSION_2_1 = (major == 2 && minor >= 1) || major > 2;
    GLAD_GL_VERSION_3_0 = (major == 3 && minor >= 0) || major > 3;
    GLAD_GL_VERSION_3_1 = (major == 3 && minor >= 1) || major > 3;
    GLAD_GL_VERSION_3_2 = (major == 3 && minor >= 2) || major > 3;

    return GLAD_MAKE_VERSION(major, minor);
}

int gladLoadGLUserPtr( GLADuserptrloadfunc load, void *userptr) {
    int version;

    glGetString = (PFNGLGETSTRINGPROC) load("glGetString", userptr);
    if(glGetString == NULL) return 0;
    if(glGetString(GL_VERSION) == NULL) return 0;
    version = glad_gl_find_core_gl();

    glad_gl_load_GL_VERSION_1_0(load, userptr);
    glad_gl_load_GL_VERSION_1_1(load, userptr);
    glad_gl_load_GL_VERSION_1_2(load, userptr);
    glad_gl_load_GL_VERSION_1_3(load, userptr);
    glad_gl_load_GL_VERSION_1_4(load, userptr);
    glad_gl_load_GL_VERSION_1_5(load, userptr);
    glad_gl_load_GL_VERSION_2_0(load, userptr);
    glad_gl_load_GL_VERSION_2_1(load, userptr);
    glad_gl_load_GL_VERSION_3_0(load, userptr);
    glad_gl_load_GL_VERSION_3_1(load, userptr);
    glad_gl_load_GL_VERSION_3_2(load, userptr);

    if (!glad_gl_find_extensions_gl(version)) return 0;



    return version;
}


int gladLoadGL( GLADloadfunc load) {
    return gladLoadGLUserPtr( glad_gl_get_proc_from_userptr, GLAD_GNUC_EXTENSION (void*) load);
}




