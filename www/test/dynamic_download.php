<?php 
$path="uploads/";
$name=$_GET["file"];
if(!file_exists($path.$name)){
    Header("Content-type:text/html;charset=utf-8");
echo "File not found.</br>";
exit;
}else{
$file=fopen($path.$name,"r");
Header("Content-type:application/octet-stream");
Header("Accept-Ranges:bytes");
Header("Content-Disposition:attachment;filename=".$name);
echo fread($file, filesize($path.$name));
fclose($file);
}
?>