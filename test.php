<html>
<head>
<title>
Cropicon - generating test
</title>
<body style='background:#eeeeee;'>


<h3><a href=''>Cropicon Test <?php print( time() ); ?></a></h3>

<?php
$hashvalue = hash("sha256","test" . rand(0,200000) );
printf("SHA256-hash: <strong>".$hashvalue."</strong>");
printf("<hr>");

$ret  = shell_exec("./cropicon.o ".$hashvalue." mycropicon.svg ");
//var_dump($ret);


//$buffer = file_get_contents("mycropicon.svg");
//printf($buffer);
?>

<div style='text-align:center;margin-top:50px;'>
<img src='./mycropicon.svg' width='300px'><br>
</div>

</body>
</html>