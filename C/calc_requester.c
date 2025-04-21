#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BASE_URL "http://localhost:8080/api/calculadora/"
#define MAX_URL_SIZE 256
#define MAX_OP_SIZE 15
#define MAX_NUM_SIZE 30
#define DEBUG

int send_request(char*);
char* get_request(const char*, const char*, const char*);
void param_err();
int post_register(const char*, const char*);

int main(int argc, char** argv) {
	if (argc != 4) {
		param_err();
		return 1;
	}

	const char* operacion = argv[1];
	const char* arg1 = argv[2];
	const char* arg2 = argv[3];

	if (strcmp(operacion, "registro") == 0) {
		return post_register(arg1, arg2);
	}

	if (strcmp(operacion, "suma") != 0 && strcmp(operacion, "resta") != 0 &&
		 strcmp(operacion, "multiplicacion") != 0 && strcmp(operacion, "division") != 0) {
		param_err();
		return 1;
	}

	char* url;
	if ((url = get_request(operacion, arg1, arg2)) == NULL) {
		fprintf(stderr, "Error: Fallo al construir url\n");
		return 1;
	}

	#ifdef DEBUG
	fprintf(stderr, "DEBUG: URL -> %s\n", url);
	#endif

	int status = send_request(url);
	free(url);

	if (status != 0) {
		fprintf(stderr, "La solicitud fallo con error\n");
		return 1;
	}

	return 0;
}

char* post_request() {
	char* url;
	int url_size;

	if ((url = malloc(MAX_URL_SIZE * sizeof(char))) == NULL) {
		return NULL;
	}

	url_size = snprintf(url, MAX_URL_SIZE, "%sregistro", BASE_URL);

	if (url_size < 0 || url_size >= MAX_URL_SIZE) {
		free(url);
		return NULL;
	}

	return url;
}

int post_register(const char* usr, const char* pass) {
	CURL *curl;
	CURLcode res;
	char* post_fields;
	char* url = post_request();

	if ((post_fields = malloc(MAX_URL_SIZE * sizeof(char))) == NULL) {
		return -1;
	}

	snprintf(post_fields, MAX_URL_SIZE, "user=%s&pass=%s", usr, pass);


	#ifdef DEBUG
	fprintf(stderr, "DEBUG: URL -> %s\n", url);
	fprintf(stderr, "DEBUG: post_request -> %s\n", post_fields);
	#endif

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);

		if ((res = curl_easy_perform(curl)) != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			return 1;
		}
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	return 0;
}

void param_err() {
	fprintf(stderr, "PARAM-ERROR: Use -> <operacion> <num1> <num2>\n");
	fprintf(stderr, "\toperacion: suma, resta, multiplicacion, division\n");
	fprintf(stderr, "Use -> registro <usuario> <constraseña>\n");
	fprintf(stderr, "Use -> usuario\n");
}

char* get_request(const char* operacion, const char* num1, const char* num2) {
	char* url;
	int url_size;

	if ((url = malloc(MAX_URL_SIZE * sizeof(char))) == NULL) {
		return NULL;
	}

	url_size = snprintf(url, MAX_URL_SIZE, "%s%s?num1=%s&num2=%s", BASE_URL, operacion, num1, num2);

	if (url_size < 0 || url_size >= MAX_URL_SIZE) {
		free(url);
		return NULL;
	}

	return url;
}

int send_request(char* url) {
	CURL* curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			curl_easy_cleanup(curl);
			return 1;
		}
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
	return 0;
}
