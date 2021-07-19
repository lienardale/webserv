<?php  
	$myfile = fopen("index.html", "a") or die("Unable to open file!");
	$txt = "POSTED!";
	fwrite($myfile, "\r\n". $txt);
	fclose($myfile);
?>  
