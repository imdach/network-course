package wireshark;

import kotlin.Pair;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

import java.io.IOException;
import java.util.Iterator;

/**
 * Created By Arthur Zhang at 2022/2/22
 */
public class H2Test {
    public static void main(String[] args) throws IOException {
        OkHttpClient client = new OkHttpClient().newBuilder().build();
        Request request = new Request.Builder().url("https://license.bytello.com/licenseAdmin/test").method("GET", null).build();
        Response response = client.newCall(request).execute();
        Iterator<Pair<String, String>> ite = response.headers().iterator();
        while (ite.hasNext()) {
            Pair<String, String> a = ite.next();
            System.out.println(a.component1() + ":" + a.component2());
        }
        System.out.println(response.body().string());
    }
}
