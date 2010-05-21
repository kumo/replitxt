#!/boot/home/config/bin/perl

@icons=("monkey fish princess","wonderful ginger tabby", "give me a hug", "kiss time");
$index=0; # the current icon
$direction=1; # the index goes 0,1,2,1,0,1,2 etc


while (1==1) {
  # use the cool BeOS "icon-as-an-attribute" to change icon
  `replitxt --text 126 "$icons[$index]"`;
  
  sleep (rand(5)+1); # sleep for a random amount of time

  # get the next icon
  $index += $direction;
  
  # bounds checking
  $direction = -1 if ($index == 3);
  $direction = 1 if ($index == 0);
}