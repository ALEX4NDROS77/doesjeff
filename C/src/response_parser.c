#include <string.h>
#include <response_parser.h>
#define iCAMPO_LEN 18
#define iCAMPOS 39

static size_t GetString(char* pclJson, jsmntok_t* ptlToken, char* pclDest) {
	return snprintf(pclDest, ptlToken->end - ptlToken->start, "%s", pclJson + ptlToken->start);
}

static size_t GetObjLen(jsmntok_t* ptlToken) {
	return (size_t)(ptlToken->end - ptlToken->start);
}

size_t ParseaResponse(char* pclContents, size_t uiSize, size_t uiNMemb, void* pvlResponse) {
	int ilReadTokens;
	char** pclKey = calloc(iCAMPOS, sizeof(char*));
	int* pilOffset = calloc(iCAMPOS, sizeof(int));
	TResponse* ptlResponse = (TResponse*)pvlResponse;
	jsmn_parser tlParser;
	jsmntok_t tlTokens[iCAMPOS]; // Se esperan 39 tokens como máximo
	size_t uiRealSize = uiSize * uiNMemb;

	jsmn_init(&tlParser);
	ilReadTokens = jsmn_parse(&tlParser, pclContents, uiRealSize, tlTokens, sizeof(tlTokens) / sizeof(tlTokens[0]));

	if (ilReadTokens < 0) {
		StmLog(sERROR,__func__,"Fallo al parsear Json [%d]\n", ilReadTokens);
		return GFI_ERRC_FATAL;
	}

	if (ilReadTokens < 1 || tlTokens[0].type != JSMN_OBJECT) {
		StmLog(sERROR,__func__,"Respuesta no es un objeto Json\n");
		return GFI_ERRC_FATAL;
	}

	if (IteraTokens(pclContents, &tlTokens, ilReadTokens,&tlMap) != GFI_ERRC_OK) {
		StmLog(sERROR,__func__,"Fallo en la iteración de tokens\n");
		return GFI_ERRC_FATAL;
	}
}

static int IteraTokens(char* pclJson, jsmntok_t* ptlTokens, int ilReadTokens, char** pclKey, int* pilOffset) {
	int ilResult = GFI_ERRC_OK;

	for (size_t uilIdx = 1; uilIdx < ilReadTokens; uilIdx++) {
		char clNombre[iCAMPO_LEN + 1];
		JsonValue nlJsonValue;
		size_t uilNombreLen;
		size_t uilValorLen;

		uilNombreLen = GetString(pclJson, ptlTokens+uilIdx, clNombre);
		jsmntype_t tlTipo = (ptlTokens + uilIdx)->type;

		switch (tlTipo) {
			case JSMN_STRING:
				uilValorLen = GetString(pclJson, ptlTokens+uilIdx+1, nlJsonValue.pcStr);
				clNombre[uilNombreLen] = '\0';
				break;
			case JSMN_OBJECT:
				uilValorLen = GetObjLen(ptlTokensúilIdx+1);
				break;
			case JSMN_PRIMITIVE:

				if (strcmp((ptlTokens+uilIdx)->start, "OK") == 0) {
					nlJsonValue.iBool = pclJson+(ptlTokens+uilIdx+1)->start == 't';
				} else {
					nlJsonValue.dDouble = strtod(pclJson+(ptlTokens+uilIdx+1)->start, NULL);
				}
				uilValorLen = sizeof(double);
				break;
		}
	}
}
