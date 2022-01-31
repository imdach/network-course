package coroutine

/**
 * Created By Arthur Zhang at 2022/1/27
 */
class Form {

}

fun main() {

}

fun postForm(form: Form) {
    val token = getUserToken()
    val result = submitPost(token, form)
    processResult(result)
}

fun processResult(result: Result) {
    TODO("Not yet implemented")
}

class Result {

}

fun submitPost(token: String, form: Form): Result {
    TODO()
}

fun getUserToken(): String {
    TODO()
}

fun postFormAsync(form: Form) {
    getUserTokenAsync { token ->
        submitPostAsync(token, form) { result ->
            processResult(result)
        }
    }
}

fun getUserTokenAsync(cb: (String) -> Unit) {
    cb("hello")
}

fun submitPostAsync(token: String, form: Form, cb: (Result) -> Unit) {
    val r = Result()
    cb(r)
}
