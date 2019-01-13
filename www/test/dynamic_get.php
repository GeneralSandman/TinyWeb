<?php
   class Client {
       public $name = "";
       public $age  = "";
   }
   $e = new Client();
   $e->name = $_GET["name"];
   $e->age  =$_GET["age"];
   echo json_encode($e);
?>