<?php
   class Client {
       public $name = "";
       public $email  = "";
   }
   $e = new Client();
   $e->name = $_POST["name"];
   $e->email  =$_POST["email"];
   print_r($_POST);
   echo json_encode($e);
?>
