#include <string.h>
#include <response_parser.h>
#define iCAMPO_LEN 18
#define iCAMPOS 39

size_t ParseaResponse(char* pclContents, size_t uiSize, size_t uiNMemb, void* pvlResponse) {
	int ilReadTokens;
	size_t uiRealSize = uiSize * uiNMemb;
	jsmn_parser tlParser;
	jsmntok_t tlTokens[iCAMPOS]; // Se esperan 39 tokens como máximo
	TResponse* ptlResponse = (TResponse*)pvlResponse;

	jsmn_init(&tlParser);
	ilReadTokens = jsmn_parse(&tlParser, pclContents, uiRealSize, tlTokens, sizeof(tlTokens) / sizeof(tlTokens[0]));

	if (ilReadTokens < 0) {
		StmLog(sERROR,__func__,"Fallo al parsear Json[%d]\n", ilReadTokens);

		if (ilReadTokens == JSMN_ERROR_NOMEM) {
			StmLog(sERROR,__func__,"[JSMN_ERROR_NOMEM]: Se recibieron más tokens de los esperados\n");
		}
		if (ilReadTokens == JSMN_ERROR_INVAL) {
			StmLog(sERROR,__func__,"[JSMN_ERROR_INVAL]: Caracter invalido en la cadena Json\n");
		}
		if (ilReadTokens == JSMN_ERROR_PART) {
			StmLog(sERROR,__func__,"[JSMN_ERROR_PART]: La cadena no es un paquete Json completo\n");
		}

		return GFI_ERRC_FATAL;
	}

	if (ilReadTokens < 1 || tlTokens[0].type != JSMN_OBJECT) {
		StmLog(sERROR,__func__,"La cadena no es un objeto Json");
		return GFI_ERRC_FATAL;
	}

	if (IteraTokens(pclContents, &tlTokens, ilReadTokens, &tlMap, &ptlResponse) != GFI_ERRC_OK) {
		// TODO: Agregar log de error
		return GFI_ERRC_FATAL;
	}
}

static int IteraTokens(char* pclJson, jsmntok_t* ptlTokens, int ilReadTokens, TMap &tlMap, TResponse &ptlResponse) {
	int ilResult = GFI_ERRC_OK;

	for (size_t uilIdx = 1; uilIdx <= ilReadTokens; uilIdx += 2) {
		JsonValue nlJsonValue;
		char* pclNombre;
		jsmntype_t tlTipo;
		size_t uilNombreLen;
		size_t uilOffset;
		size_t uilValorLen;

		pclNombre = pclJson+ptlTokens[uilIdx].start;
		tlTipo = ptlTokens[uilIdx+1].type;
		uilNombreLen = (size_t)(ptlTokens[uilIdx].end - ptlTokens[uilIdx].start);
		uilValorLen = (size_t)(ptlTokens[uilIdx+1].end - ptlTokens[uilIdx+1].start);
		uilOffset = ; // Funcion para obtener el offset del campo en la estructura TResponse segun el nombre del campo

		switch (tlTipo) {
			case JSMN_OBJECT:
				if (uilValorLen <= 1) { // Qué hacer si el objeto está vacío
					StmLog(sINFO,__func__,"Token %d es un objeto vacio: %.*s\n", uilIdx, (int)uilNombreLen, pclNombre);
				}
				break;
				// Es necesario el uso de la union?
			case JSMN_STRING:
				nlJsonValue.pcStr = pclJson+ptlTokens[uilIdx+1].start;
				memcpy(ptlResponse + uilOffset, nlJsonValue.pcStr, uilValorLen);
				((char*)ptlResponse)[uilOffset + uilValorLen] = '\0'; // Asegurar que la cadena termina en nulo
				break;
			case JSMN_PRIMITIVE:
				if (*(pclJson+ptlTokens[uilIdx+1].start) == 't' || *(pclJson+ptlTokens[uilIdx+1].start) == 'f') {
					nlJsonValue.iBool = (*(pclJson+ptlTokens[uilIdx+1].start) == 't') ? 1 : 0;
					memcpy(ptlResponse + uilOffset, &nlJsonValue.iBool, uilValorLen);
					break;
				}

				nlJsonValue.dDouble = strtod(pclJson+ptlTokens[uilIdx+1].start, NULL);
				memcpy(ptlResponse + uilOffset, &nlJsonValue.dDouble, uilValorLen);
				break;

			default:
				ilResult = GFI_ERRC_FATAL;
				StmLog(sERROR,__func__,"Token %d no reconocido: %d\n", uilIdx, tlTipo);
				return ilResult;
		}
	}
	return ilResult;
}
