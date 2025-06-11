#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#define iCUENTA_LEN 11
#define iIMPORTE_LEN 6
#define iJSON_REQUEST_LEN 128
#define sABONO_REQUEST "{\"cuenta\":\"%s\",\"importe\":\"%s\"}"

typedef struct {
	char[iCUENTA_LEN] cCuenta;
	char[iIMPORTE_LEN] cImporte;
}TAbonoRequest;

int ConfigCurl(CURL* ptlCurl, char* pclEndpoint);
int ArmaRequest(TAbonoRequest* ptlAbonoRequest, char* pclJsonRequest);

int main(int argc, char* argv[]) {
	int ilResult = 0;
	TAbonoRequest tlAbonoRequest;
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <cuenta> <importe>\n", argv[0]);
		return 1;
	}

	snprintf(tlAbonoRequest.cCuenta, iCuenta_LEN, "%s", argv[1]);
	snprintf(tlAbonoRequest.cImporte, iIMPORTE_LEN, "%s", argv[2]);
	
	if (ilResult != 0) {
		fprintf(stderr, "Error: %s\n", strerror(ilResult));
		return ilResult;
	}
	ilResult = AbonoSender("http://localhost:8080/abono", &tlAbonoRequest);
	return ilResult;
}

int ArmaRequest(TAbonoRequest* ptlAbonoRequest, char* pclJsonRequest) {
	if (ptlAbonoRequest == NULL || pclJsonRequest == NULL) {
		return -1; // Error: Invalid request pointer
	}
	if (snprintf(pclJsonRequest, iJSON_REQUEST_LEN, sABONO_REQUEST, ptlAbonoRequest->cCuenta, ptlAbonoRequest->cImporte) < 0) {
		return -2; // Error: snprintf failed
	}
	return 0;
}

int AbonoSender(char* pclEndpoint, TAbonoRequest* ptlAbonoRequest) {
	int ilResult = 0;
	char[iJSON_REQUEST_LEN] clJsonRequest = {'\0'};
	CURL* ptlCurl = curl_easy_init();
	CURLcode tlResult;

	if ((ilResult = ConfigCurl(ptlCurl, pclEndpoint)) == NULL) {
		fprintf(stderr, "Error configuring CURL: %d\n", ilResult);
		curl_easy_cleanup(ptlCurl);
		return ilResult;
	}

	if ((ilResult = ArmaRequest(&tlAbonoRequest, clJsonRequest)) != 0) {
		fprintf(stderr, "Error creating request: %d\n", ilResult);
		curl_easy_cleanup(ptlCurl);
		return ilResult;
	}

	tlResult = curl_easy_perform(ptlCurl);
}

struct curl_slist* ConfigCurl(CURL* ptlCurl, char* pclEndpoint) {
	struct curl_slist* ptlHeaders = NULL;
	if (ptlCurl == NULL || pclEndpoint == NULL) {
		return -1; // Error: Invalid parameters
	}
	curl_easy_setopt(ptlCurl, CURLOPT_URL, pclEndpoint);
	curl_easy_setopt(ptlCurl, CURLOPT_POST, 1L);
	ptlHeaders = curl_slist_append(ptlHeaders, "Content-Type: application/json");
	curl_easy_setopt(ptlCurl, CURLOPT_HTTPHEADER, ptlHeaders); // Set headers for JSON content
	return ptlHeaders; // Success
}
