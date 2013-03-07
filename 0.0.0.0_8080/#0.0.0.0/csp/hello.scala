// ============================================================================
// C servlet sample for the G-WAN Web Application Server (http://trustleap.ch/)
// ----------------------------------------------------------------------------
// hello.scala: G-WAN now talks Scala
// ============================================================================
import api.Gwan

object hello
{
  def jmain(env: Long, args: Array[String]) : Int = 
  {
     Gwan.xbufCat(Gwan.getReply(env), "Hello World");
     return 200;
  }
}
// ============================================================================
// End of Source Code
// ============================================================================

