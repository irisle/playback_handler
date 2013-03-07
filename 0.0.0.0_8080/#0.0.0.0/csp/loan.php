<?
   //:: http://10.10.2.6/loan.php?name=Eva&amount=10000&rate=3.5&term=1 ::

   //:: no query parameters were provided, redirect client to "loan.html" ::
   //:: **MISSING ::
   
   //:: convert PHP's command-line arguments into PHP's URI parameters ::
   if(PHP_SAPI==="cli") :
      parse_str(implode('&', array_slice($argv, 1)), $_GET);
   endif;

   //:: if we have query parameters, we process a GET/POST form ::
   $name = isset($_GET['name']) ? $_GET['name'] : "Eva";
   if($name!='') :
       $monthList = array("January", "February", "March", "April", "May", 
                          "June", "July", "August", "September", "October",
                          "November", "December", "");
       $start  = microtime(true);       
       $amount = isset($_GET['amount']) ? floatval(trim($_GET['amount'])) : 10000;
       if($amount < 1) :
          $amount = 1;
       endif;
       
       $rate   = isset($_GET['rate']) ? floatval(trim($_GET['rate'])) : 3.5;
       if($rate > 19) :
          $rate = 20;
       endif;
       if($rate > 1) :
          $rate = $rate / 100.;
       else :
          if($rate < 1) :
             $rate = 1. / 100.;
          endif;
       endif;
       
       $term   = isset($_GET['term']) ? floatval(trim($_GET['term'])) : 1;
       if($term < 0.1) :
          $term = 1. / 12.;
       endif;
       
       $months = ($term>32)?$term:$term*12;
       $name   = addslashes($name);
       $year   = 1;
       $lastpayment = 1;

       //:: all litteral strings provided by a client must be escaped this way ::
       //:: if you inject them into an HTML page ::
       htmlspecialchars($name);

       //:: filter input data to avoid all the useless/nasty cases ::
       //:: **MISSING ::

       //:: calculate the monthly payment amount ::
       $payment = $amount*$rate/12*pow(1+$rate/12, $term*12)
                               /  (pow(1+$rate/12, $term*12)-1);
       $cost    = ($term*12*$payment)-$amount;

       //:: build the top of our HTML page ::
       $result  = "<html><head><title>Loan Calculator</title>" .
                  "<meta http-equiv=\"Content-Type\" content=\"text/html;" .
                  " charset=windows-1252\">" .
                  "<link href=\"imgs/style.css\" rel=\"stylesheet\" " .
                  "type=\"text/css\"></head>" .
                  "<body bgcolor=\"White\" style=\"padding:16px\"><p>" .
                  "<h1>Loan Calculator</h1></p><br>" .
                  "<h2>Dear $name, your loan goes as follows:</h2><br>";
       $result .= sprintf("<br><table class=\"clean\" width=240px>" .
                   "<tr><th>loan</th><th>details</th></tr>" .
                   "<tr class=\"d1\"><td>Amount</td><td>%s</td></tr>" .
                   "<tr class=\"d0\"><td>Rate</td><td>%.2f%%</td></tr>" .
                   "<tr class=\"d1\"><td>Term</td><td>%u %s(s)</td></tr>" .
                   "<tr class=\"d0\"><td>Cost</td><td>%.2f (%.2f%%)</td></tr>" .
                   "</table>", number_format($amount, 2), $rate*100, 
                   $term,
                   //:: **MISSING uceil(12*term), ::
                   ($term<36)?"year":"month",
                   $cost, 100/($amount/$cost));
       
	   $result .= sprintf("<br><table class=\"clean\" width=112px>" .
               "<tr class=\"d1\"><td><b>YEAR %u</b></td></tr></table>" .
               "<table class=\"clean\" width=550px>" .
               "<tr><th>month</th><th>payment</th><th>interest</th>" .
               "<th>principal</th><th>balance</th></tr>",
               $year);

	  for($month=1; $payment>0; $month++) : //:: output monthly payments ::
   	   $interest = ($amount*$rate)/12;
		   if($amount>$payment) :
             $amount    = ($amount-$payment)+$interest;
	          $principal = $payment-$interest;
           else : //:: calculate last payment ::
              if($lastpayment) :
                 $lastpayment = 0;
                 $payment     = $amount;
                 $principal   = $amount-$interest;
                 $amount      = 0;
              else : //:: all payments are done, just padd the table ::
                 $amount    = 0;
                 $payment   = 0;
                 $interest  = 0;
                 $principal = 0;
              endif;
         endif;

         $result .= sprintf("<tr class=\"d%u\"><td>%s</td><td>%s</td><td>" .
                          "%s</td><td>%s</td><td>%s</td></tr>",
                          $month%2, $monthList[$month-1], 
                          number_format($payment, 2), 
                          number_format($interest, 2), 
                          number_format($principal, 2), 
                          number_format($amount, 2));

		   if($month==12) :
            if($amount) :
			   $month=0; $year++;
	           $result .= sprintf("</table><br><table class=\"clean\" " .
                       "width=112px><tr class=\"d1\"><td><b>YEAR %u</b>" .
                       "</td></tr></table><table class=\"clean\" width=550px>" .
                       "<tr><th>month</th><th>payment</th><th>interest</th>" .
                       "<th>principal</th><th>balance</th></tr>",
                       $year);
           endif;
        endif;

    endfor;

    //:: time the process and close the HTML page ::
    $result .= sprintf("</table><br>This page was generated in %s ms." .
                       "<br>(on a 3GHz CPU 1 ms = 3,000,000 cycles)" .
                       "<br></body></html>",
                       number_format((microtime(true)-$start)*1000), 2);
    echo $result;
    exit(200);
    endif;
?>
<br>
