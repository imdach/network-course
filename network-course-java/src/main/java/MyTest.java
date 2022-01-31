/**
 * Created By Arthur Zhang at 2022/1/28
 */
public class MyTest {
    public static int func() {
        try {
            int a = 1 / 0;
            return 0;
        } catch (Exception e) {
            int b = 1 / 0;
        } finally {
            return 2;
        }
    }
}
