#!/boot/home/config/bin/perl
#
# Timer v1.0
#
# Frank
#
# a program to count down until (surprise, surprise) thursday 2nd september at 11.30am. uses
# external beos/posix compliance commands so no working under micro$oft in any form at the
# moment. works but crazy thing takes up over 90% kernel time...
# 27th September 1999

$lastname="xmas.pl";
$enddate=`date -d"25 dec 2001" +%s`;

while (1==1) {
  $todaydate=`date +%s`;

  $diff=$enddate-$todaydate;

  $seconds=$diff%60;
  $minutes=(($diff/(60))%60);
  $hours=($diff/(60*60))%24;
  $days=($diff/(60*60*24));#%7 - only if weeks are included;
  $months=($diff/(60*60*24*12));

#  $newname=" $days $hours:$minutes:$seconds";
  $newname="$days{d} ";
  if ($hours < 10) {
    $newname=$newname."0";
  }
  $newname=$newname."$hours:";
  if ($minutes < 10) {
    $newname=$newname."0";
  }
  $newname=$newname."$minutes:";

  if ($seconds < 10) {
    $newname=$newname."0";
  }
  $newname=$newname."$seconds";

  #print "\$newname = $newname\n";
  #print "replitxt --text 123 $newname\n";
  `replitxt --text 126 $newname`;
  `sleep 1s`;

  $lastname=$newname;
  #print "\$lastname = $lastname\n";
}
#`alert "time to go:\n\n\t$days(d) $hours:$minutes:$seconds" "hooray\!"`
