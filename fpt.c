#include <stdio.h>
#include <curl/curl.h>

// Hàm callback để ghi dữ liệu vào file
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    FILE *file = (FILE *)userp;
    return fwrite(contents, size, nmemb, file);
}

int main(void) {
    CURL *curl;
    CURLcode res;
    FILE *file;
    
    // Khởi tạo cURL
    curl = curl_easy_init();
    if (curl) {
        // Đặt URL của server FTP
        curl_easy_setopt(curl, CURLOPT_URL, "ftp://ftp.example.com/path/to/remote_file.txt");
        
        // Đặt tên người dùng và mật khẩu
        curl_easy_setopt(curl, CURLOPT_USERNAME, "username");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "password");
        
        // Chọn chế độ tải lên (EPSV hoặc PASV)
        curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
        
        // Mở file cần upload
        file = fopen("local_file.txt", "rb");
        if (file) {
            // Đặt callback để ghi dữ liệu vào file
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            
            // Thực hiện upload file
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "Upload failed: %s\n", curl_easy_strerror(res));
            }
            
            // Đóng file
            fclose(file);
        }
        
        // Cleanup curl
        curl_easy_cleanup(curl);
    }
    
    return 0;
}
