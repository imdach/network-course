package wireshark;

import okhttp3.OkHttpClient;
import okhttp3.Response;

import java.io.IOException;

public class HttpDownloadUtility {

    static OkHttpClient client = new OkHttpClient();

    public static void main(String[] args) throws IOException {
        String url = "https://book.douban.com/";

        okhttp3.Request request = new okhttp3.Request.Builder().url(url).build();

        try (Response response = client.newCall(request).execute()) {
            System.out.println(response.body().string());
        }
    }
}
