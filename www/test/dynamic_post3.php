<?php
   class Client {
       public $name = "";
       public $email  = "";
       public $function = "";
   }
   $e = new Client();
   $e->name = $_POST["name"];
   $e->email  =$_POST["email"];
   $e->function = "dynamic_post3.php";
   echo json_encode($e);
?>
