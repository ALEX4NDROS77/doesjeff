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
	int iBool;
	double dDouble;
	char* pcStr;
} JsonValue;

typedef struct TResponse {
char* pcMsgError;
char cCodeError[iERRC_LEN + 1];

struct {
	char cLegacyCode[iLEGACY_LEN + 1];
	char* pcExtendedCode;
	char* pcResponseCode;
	char* pcResponseMessage;
	char cCode[iERRC_LEN + 1];
	char* pcMessage;
	char* pcErrorCode;
} tMoreInfo;

struct {
	double dImporte;
	char cCodOper[iCOD_OPER_LEN + 1];
	char cFecha[iFECHA_LEN + 1];
	char cFchHr[iFECHA_LEN + 1];
	char* pcFolio;
	char cCCC[iCCC_LEN + 1];
	char cCodDivisa[iDIVISA_LEN + 1];
	char* pcDescDivisa;
	char* pcDescCod;
	char cCodAppNio[iAPP_NIO_LEN + 1];
	char cTerminalNIO[iTERMINAL_LEN + 1];
	char cFchNIO[iFECHA_LEN + 1];
	char cHraNIO[iHORA_LEN + 1];
	char* pcCheque;
	char cFchCont[iFECHA_LEN + 1];
	char* pcCodComis;
	double dImporteComis;
	double dPorcentComis;
	char cCodImpuesto[iCOD_IMP_LEN + 1];
	double dImporteImp;
	char cCentroOper[iCEN_OPER_LEN + 1];
	char* pcDescCentro;
	char cTerminalOper[iTERMINAL_LEN + 1];
	char cCodTransOper[iCOD_TRANS_OPE + 1];
	char cHoraOper[i24_HORA_LEN + 1];
} tResponse;

	int iFlagOk; // Bool
	char* pcRes2JsonStr; // Response to JSON string
}
