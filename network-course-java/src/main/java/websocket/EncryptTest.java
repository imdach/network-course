package websocket;

import org.apache.commons.codec.DecoderException;
import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.binary.Hex;
import org.apache.commons.codec.digest.DigestUtils;

/**
 * Created By Arthur Zhang at 2022/4/18
 */
public class EncryptTest {
    public static void main(String[] args) throws DecoderException {

        String Sec_WebSocket_Key = "7TjGCyY0lUzytpilR4YOpA==";
        String res = calc(Sec_WebSocket_Key);
        System.out.println(res);
    }

    public static String calc(String key) {
        String toSha = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
        String sha1Hex = DigestUtils.sha1Hex(toSha);
        try {
            return Base64.encodeBase64String(Hex.decodeHex(sha1Hex));
        } catch (DecoderException e) {
        }
        return null;
    }
}
