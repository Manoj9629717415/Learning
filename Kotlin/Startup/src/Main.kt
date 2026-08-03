import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.withContext
import sun.rmi.server.Dispatcher
import java.util.ArrayList
import kotlin.collections.sum
import kotlin.concurrent.thread
import kotlin.coroutines.CoroutineContext


//TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
fun main()= runBlocking {
    val name = "Kotlin"

    val inte : Int = 6

    val b = inte.toByte()

    val c: String = b.toString()

    val value = listOf(1,2,3,4,5,6,7,8,9,10)

    //println("b= " + b + ", c = "+c)

    var d: Int = 5

    //println("d = "+ d)

    d = 6

   // println("d = "+ d)

    //println(" c + d = ${c + d }")

    if( d in 1..100)
    {
       // println("good ratio")
    }

    for (i in 1..5) {
      //  println("i = $i")
    }

//    when (d) {
//        0 -> println("empty tank")
//        in 1..16 -> println("small fish")
//        else -> println("big Fish")
//    }

    val fruit_list = listOf("banana","apple",1)

//    for(i in fruit_list)
//    {
//        println(i)
//    }

    // thread { blockingcode()  }

    CoroutineScope(Dispatchers.IO).launch{
       launch {
           cookRice()
           println("cookRice finished")
       }
        launch {
            cookChícken()
            println("cookChícken finished")

        }
    }.join()

    val value1 = listOf(1,2,3,4,5,6,7,8,9,10)




    IntArray.sumAsync

    printstring(arrayOf("Hello","world"))

    println(value.filter { it.toInt() % 2 == 0 })

    val lazymap = value.asSequence().filter { it % 2 != 0 }.map { it }

    println(lazymap.toList())

    val increasetemp: ( Int ) -> Int  = { variab -> variab+1 }

    println(increasetemp(10))

    boilertemp(20,increasetemp)
}

fun printstring(text: Array<String>) : String
{
    for(i in text)
    {
        println(i)
    }
    return text[0]
}

fun boilertemp(com:Int,incre:(Int)->Int)
{
    println(incre(com))
}

fun blockingcode()
{
    println("Blockingcode started")
    (1..5000000).map { it *it }
    println("Blockingcode finished")
}

suspend fun cookRice()
{
    delay(3000)
   // (1..5000000).map { it *it }
}

suspend fun cookChícken()
{
    delay(4000)
}



    suspend fun IntArray.sumAsync(): Int =
        withContext(Dispatchers.Default) {
            sum()
        }
viewModelScope.launch {
    IntArray.sumAsync(): Int =
    withContext(Dispatchers.Default) {
        sum()
    }
}
```

