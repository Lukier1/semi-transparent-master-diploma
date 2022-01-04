#include "stdafx.h"

#include <GLUtils.h>

void APIENTRY debug_gl_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, GLvoid* userParam
) {
	char src_str[2048]; /* source */
	char type_str[2048]; /* type */
	char sev_str[2048]; /* severity */

	switch (source) {
	case 0x8246:
		strcpy_s(src_str, sizeof(src_str), "API");
		break;
	case 0x8247:
		strcpy_s(src_str, sizeof(src_str), "WINDOW_SYSTEM");
		break;
	case 0x8248:
		strcpy_s(src_str, sizeof(src_str), "SHADER_COMPILER");
		break;
	case 0x8249:
		strcpy_s(src_str, sizeof(src_str), "THIRD_PARTY");
		break;
	case 0x824A:
		strcpy_s(src_str, sizeof(src_str), "APPLICATION");
		break;
	case 0x824B:
		strcpy_s(src_str, sizeof(src_str), "OTHER");
		break;
	default:
		strcpy_s(src_str, sizeof(src_str), "undefined");
		break;
	}

	switch (type) { //type
	case 0x824C:
		strcpy_s(type_str, sizeof(src_str), "ERROR");
		break;
	case 0x824D:
		strcpy_s(type_str, sizeof(src_str), "DEPRECATED_BEHAVIOR");
		break;
	case 0x824E:
		strcpy_s(type_str, sizeof(src_str), "UNDEFINED_BEHAVIOR");
		break;
	case 0x824F:
		strcpy_s(type_str, sizeof(src_str), "PORTABILITY");
		break;
	case 0x8250:
		strcpy_s(type_str, sizeof(src_str), "PERFORMANCE");
		break;
	case 0x8251:
		strcpy_s(type_str, sizeof(src_str), "OTHER");
		break;
	case 0x8268:
		strcpy_s(type_str, sizeof(src_str), "MARKER");
		break;
	case 0x8269:
		strcpy_s(type_str, sizeof(src_str), "PUSH_GROUP");
		break;
	case 0x826A:
		strcpy_s(type_str, sizeof(src_str), "POP_GROUP");
		break;
	default:
		strcpy_s(type_str, sizeof(src_str), "undefined");
		break;
	}

	switch (severity) {
	case 0x9146:
		strcpy_s(sev_str, sizeof(src_str), "HIGH");
		break;
	case 0x9147:
		strcpy_s(sev_str, sizeof(src_str), "MEDIUM");
		break;
	case 0x9148:
		strcpy_s(sev_str, sizeof(src_str), "LOW");
		break;
	case 0x826B:
		strcpy_s(sev_str, sizeof(src_str), "NOTIFICATION");
		break;
	default:
		strcpy_s(sev_str, sizeof(src_str), "undefined");
		break;
	}

	fprintf(
		stderr,
		"source: %s type: %s id: %u severity: %s length: %i message: %s userParam: %i\n",
		src_str,
		type_str,
		id,
		sev_str,
		length,
		message,
		*(int*)userParam
	);
}