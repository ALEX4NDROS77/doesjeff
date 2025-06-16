#idndef PARSER
#define PARSER 1
#define iERRC_LEN
#define iLEGACY_LEN
#define iCCC_LEN
#define iCEN_OPER_LEN
#define iAPP_NIO_LEN
#define iDIVISA_LEN
#define iCOD_IMP_LEN
#define iCOD_OPER_LEN
#define iCOD_TRANS_OPE
#define iFECHA_LEN
#define i24_HORA_LEN
#define iHORA_LEN
#define iTERMINAL_LEN
#define iCOMI_COD_LEN
#define iDESC_DIV_LEN

typedef union JsonValue {
	double dDouble; // 8 bytes
	char* pcStr; // 8 bytes
	short iBool; // 2 bytes
} JsonValue; // 18 bytes, 24 w/padding

typedef struct TResponse {
	char* pcMsgError; // 8 bytes
	char* pcRes2JsonStr; // Response to JSON string // 8 bytes

	struct {
		char* pcErrorCode; // 8 bytes
		char* pcExtendedCode; // 8 bytes
		char* pcMessage; // 8 bytes
		char* pcResponseCode; // 8 bytes
		char* pcResponseMessage; // 8 bytes
		char cCode[iERRC_LEN + 1]; // undefined length
		char cLegacyCode[iLEGACY_LEN + 1]; // undefined length
	} tMoreInfo; // 40 bytes + undefined array sizes

	struct {
		char* pcCheque; // 8 bytes
		char* pcCodComis; // 8 bytes
		char* pcDescCentro; // 8 bytes
		char* pcDescCod; // 8 bytes
		char* pcDescDivisa; // 8 bytes
		char* pcFolio; // 8 bytes
		double dImporte; // 8 bytes
		double dImporteComis; // 8 bytes
		double dImporteImp; // 8 bytes
		double dPorcentComis; // 8 bytes
		char cCCC[iCCC_LEN + 1]; // undefined length
		char cCentroOper[iCEN_OPER_LEN + 1]; // undefined length
		char cCodAppNio[iAPP_NIO_LEN + 1]; // undefined length
		char cCodDivisa[iDIVISA_LEN + 1]; // undefined length
		char cCodImpuesto[iCOD_IMP_LEN + 1]; // undefined length
		char cCodOper[iCOD_OPER_LEN + 1]; // undefined length
		char cCodTransOper[iCOD_TRANS_OPE + 1]; // undefined length
		char cFchCont[iFECHA_LEN + 1]; // undefined length
		char cFchHr[iFECHA_LEN + 1]; // undefined length
		char cFchNIO[iFECHA_LEN + 1]; // undefined length
		char cFecha[iFECHA_LEN + 1]; // undefined length
		char cHoraOper[i24_HORA_LEN + 1]; // undefined length
		char cHraNIO[iHORA_LEN + 1]; // undefined length
		char cTerminalNIO[iTERMINAL_LEN + 1]; // undefined length
		char cTerminalOper[iTERMINAL_LEN + 1]; // undefined length
	} tResponse; // 80 bytes + undefined array sizes

	shor iFlagOk; // Bool // 2 bytes
	char cCodeError[iERRC_LEN + 1]; // undefined length
} TResponse; // 138 bytes + undefined array sizes

size_t ParseaResponse(char* pvlContents, size_t uiSize, size_t uiNMemb, void* pvlResponse);
