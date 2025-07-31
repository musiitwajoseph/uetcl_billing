<?php
ob_start();
session_start();
error_reporting(null);
include("../classes/init.inc");
//include("../classes/FeedBack.php");  
ini_set('display_errors', 1);
$xx = array(); 
if(1){
                        
    $username = $_POST['username'];
    $password = $_POST['password'];
   // $captcha = $_POST['captcha'];

    if(!empty($username) && !empty($password)){

        if(0){
            //Feedback::error("Entered Captcha code does not match");
           // $xx['message']="Error";
        }else{
            //$us = new Users();
            //$p = $password;
           
            $ad = new ActiveDirectory();
            $ad_results = $ad->login($username, $password);    
           //echo "@@@@@@@@@@@@@@@";                   
            //print_r($ad_results);
            if($ad_results['status']){
                $name = $ad_results['message']['name'];
                $email = $ad_results['message']['email'];
                $telephone = $ad_results['message']['extention'];
                $ur = $ad_results['message']['user_role'];
                $time = time();
                $db = new Db();

                $select = $db->select("SELECT * FROM sysuser WHERE user_name = '$username'");
                if($db->num_rows()){
                    extract($select[0][0]);
                }else{                                
                    $db->insert("sysuser", [
                        //"check_number"=>$username,     
                        "user_surname"=>$name,
                        "user_othername"=>'',
                        "user_email"=>$email,
                        "user_gender"=>'',
                        "user_role"=>$ur,
                        "user_name"=>$username, 
                        "user_status"=>1,       
                        //"user_password"=>$this->penc($password),
                        "user_date_added"=>$time,
                        "user_telephone"=>$telephone,
                        "user_forgot_password"=>0,
                        "user_active"=>1,
                    ]);

                    $select = $db->select("SELECT * FROM sysuser WHERE user_name = '$username'");
                    if($db->num_rows()){
                        extract($select[0][0]);
                    }
                }

            

                    //$query_string = (empty(end(explode("=", $_SERVER['QUERY_STRING']))))? "dashboard/index":end(explode("=", $_SERVER['QUERY_STRING']));
                    
                    $_SESSION['UEDCL_USER_ID'] = $user_id;
                    $_SESSION['UEDCL_ROLE_ID'] = $user_id;
                    $xx['message']="Success"; 

                    $db = new Db();

                   $update = $db->update("sysuser", ["user_last_logged_in"=>time(), "user_online"=>1, "user_last_active"=>time()], ["user_id"=>$user_id]);

//print_r($xx);
                    //print_r($db->error);
                    AuditTrail::registerTrail("LOGIN-SUCCESSFULL", $db_id="",  "LOGIN-SUCCESSFULL", "LOGIN-SUCCESSFULL");
                    
            

            }else{
                //Feedback::warning("Wrong User name or Password");

                AuditTrail::registerTrail("LOGIN-FAILED", $db_id="",  "LOGIN-FAILED", "LOGIN-FAILED: username_entered->$username AND password->$password");
                 $xx['message']="Error";
            }
        }
    }else{
        //Feedback::error("Please Enter Username, Password & Captcha Code");
        $xx['message']="Error";
    }
}

echo json_encode($xx);

