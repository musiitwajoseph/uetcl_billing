<?php ob_start(); 
error_reporting(1);
/*** This file is dynamically generated ***
█████▄ ▄████▄   █████▄ ▄████▄ ██████   ███████▄ ▄████▄ █████▄ ██ ██████ ██  ██
██  ██ ██  ██   ██  ██ ██  ██   ██     ██ ██ ██ ██  ██ ██  ██ ██ ██▄▄   ██▄▄██
██  ██ ██  ██   ██  ██ ██  ██   ██     ██ ██ ██ ██  ██ ██  ██ ██ ██▀▀    ▀▀▀██
█████▀ ▀████▀   ██  ██ ▀████▀   ██     ██ ██ ██ ▀████▀ █████▀ ██ ██     █████▀

*/

?>
<!DOCTYPE html>
<html>
<?php
   
include "classes/init.inc";

$con = "";

header("Last-Modified: Fri, 18 Aug 2023 ".date("h:i:s")." GMT");
header("Cache-Control: no-cache, must-revalidate");

?>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=Edge">
    <meta content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" name="viewport">
    <title>UETCL BILLING AND RECEIPT MANAGEMENT SYSTEM</title>
    <!-- Favicon-->
    <link rel="icon" href="<?php echo return_url().'images/uetcl.png'; ?>" type="image/x-icon">
    <!-- Custom Fonts -->
    <link href="<?php display_url(); ?>css/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">

    <!-- Bootstrap Core Css -->
    <link href="<?php display_url(); ?>css/bootstrap/css/bootstrap.css" rel="stylesheet">

    <!-- Waves Effect Css -->
    <link href="<?php display_url(); ?>css/node-waves/waves.css" rel="stylesheet" />

    <!-- Animation Css -->
    <link href="<?php display_url(); ?>css/animate-css/animate.css" rel="stylesheet" />

    <!-- Morris Chart Css-->
    <link href="<?php display_url(); ?>css/morrisjs/morris.css" rel="stylesheet" />
    
    <link rel="stylesheet" type="text/css" href="<?php display_url(); ?>DataTables/datatables.min.css"/>
    
    <!-- Bootstrap Material Datetime Picker Css -->
    <link href="<?php display_url(); ?>css/bootstrap-material-datetimepicker/css/bootstrap-material-datetimepicker.css" rel="stylesheet" />

    <!-- Bootstrap DatePicker Css -->
    <link href="<?php display_url(); ?>css/bootstrap-datepicker/css/bootstrap-datepicker.css" rel="stylesheet" />

    <link rel="stylesheet" href="<?php display_url(); ?>css/theme.default.css">

    <!-- Custom Css -->
    <link href="<?php display_url(); ?>css/style.css" rel="stylesheet">

    <!-- AdminBSB Themes. You can choose a theme from css/themes instead of get all themes -->
    <link href="<?php display_url(); ?>css/themes/all-themes.css" rel="stylesheet" />
        

    <!-- Bootstrap Core Js -->
<script type="text/javascript" charset="utf8" src="<?php display_url(); ?>jquery-3.4.1.min.js"></script>
  
<link href="https://cdnjs.cloudflare.com/ajax/libs/select2/4.0.6-rc.0/css/select2.min.css" rel="stylesheet" />
<script src="https://cdnjs.cloudflare.com/ajax/libs/select2/4.0.6-rc.0/js/select2.min.js"></script>

    <script type="text/javascript" src="<?php display_url();?>js/script.js"></script>    

    <script type="text/javascript" src="<?php display_url();?>css/bootstrap-material-datetimepicker/js/material.min.js"></script>
    
    <script type="text/javascript" src="<?php display_url();?>css/bootstrap-material-datetimepicker/js/moment-with-locales.min.js"></script>

    <link href="<?php display_url(); ?>css/print.min.css" rel="stylesheet">
    <script type="text/javascript" src="<?php display_url();?>js/print.min.js"></script>

    <script type="text/javascript" src="<?php display_url();?>css/bootstrap-material-datetimepicker/js/bootstrap-material-datetimepicker.js"></script>
    <script type="text/javascript">var totInputted=0;</script>
    
    <script type="text/javascript" src="<?php display_url();?>js/sweetalert.min.js"></script>
</head>

    <input type="hidden" id="urlPath" value="<?php echo return_url(); ?>"/>
    <?php
    if(isset($_SESSION['UETCL_USER_FORGET'])){
    ?>
    <body class="login-page ls-closed">
    <div class="login-box"><br/><br/><br/>       
        <div class="card">
            <div class="body" style="position:relative;margin-top:60px;border-radius:20px;padding-top:80px;border:2px solid blue;">
                <form id="sign_in" method="POST"> 
                    <?php 
                    if(isset($_POST['passwordremembered'])){                       
                        unset($_SESSION['UETCL_USER_FORGET']);
                        header("Location:".return_url());
                    }elseif(isset($_POST['send'])){
                        $email = $_POST['email'];

                        $errors = array();

                        if(empty($email)){
                            $errors[] = "Enter Username";
                        }

                        if(empty($errors)){
                            $db = new Db();
                            $select = $db->select("SELECT * FROM sysuser WHERE user_name = '$email'");                 
                            if($db->num_rows()){

                                extract($select[0][0]);
                                
                                $new_password = Feedback::password_generator();
                                $msg = array();
                                $msg[] = "Hello $user_surname $user_othername, ";
                                $msg[] = "";
                                $msg[] = "Your Login credentials have been changed:";
                                $msg[] = "Username: <b>$user_name</b>";
                                $msg[] = "Password: <b>$new_password</b>";
                                $msg[] = "";
                                $msg[] = "You will be required to login with the above Username and Password and then change the password to one that you will easily remember.";
                                $message[] = "Thank You.";

                                $to = $user_email;
                                $subject = "UETCL FORGOT PASSWORD";
                                $message = implode("\r \n <br/>", $msg);


                                $db = new Db();
                                $update = $db->update("sysuser", ["user_password"=>$new_password, "user_online"=>0, "user_forgot_password"=>1], [user_id=>$user_id]);

                                Feedback::sendmail($to,$subject,$message,$name);

                                Feedback::success("Please check your email to Proceed with resetting your password.");
                                Feedback::refresh("10");

                                AuditTrail::registerTrail("FORGOT PSWD - SUCCESSFULL", $db_id="",  "LOGIN-SUCCESSFULL", "LOGIN-SUCCESSFULL");

                                
                            }else{
                                Feedback::warning("Username does not exist in the system");

                                AuditTrail::registerTrail("FORGOT PSWD - FAILED", $db_id="",  "LOGIN-FAILED", "LOGIN-FAILED: username_entered->$username AND password->$password");
                            }
                        }else{
                            Feedback::errors($errors);
                        }
                    }

                     //AuditTrail::registerTrail("LOGIN - FAILED", $db_id="",  "LOGIN - SUCCESSFULL", "LOGIN - SUCCESSFULL");
                    ?>

                    <img src="<?php echo return_url().'images/uedcl.png'; ?>" style="position:absolute;top:-70px; left:30%; width:150px;"/>
                    <div class="sys_name"><span style="font-size:2em;color:blue;">UETCL</span><BR/>BILLING AND RECEIPTING SYSTEM</div>

                    <div class="msg">
                        <h5>Forgot Password?</h5>
                    </div>
                    <div class="input-group">
                        <span class="input-group-addon">
                            <i class="fa fa-fx fa-user"></i>
                        </span>
                        <div class="form-line">
                            <input type="text" class="form-control" name="email" placeholder="Enter Your Username" value="<?php echo $email; ?>" autofocus>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-xs-12">
                            <button name="send" class="btn btn-block bg-blue waves-effect" type="submit"><i class="fa fa-fx fa-plane"></i> Send</button>
                        </div>
                    </div>
                    <div class="row m-t-15 m-b--20">
                        <div class="col-xs-12 align-right"> <button name="passwordremembered" value="GOOD" class="forgot-password"><i class="fa fa-fw fa-lock"></i>
                           I have Remembered my password, Login</button>

                        </div>
                    </div>                    
                </form>
            </div>
        </div>
    </div>
    <?php }elseif(!isset($_SESSION['UEDCL_USER_ID'])){
    ?>
    <body class="login-page ls-closed">
    <div class="login-box"><br/><br/><br/>       
        <div class="card">
            <div class="body" style="position:relative;margin-top:60px;border-radius:20px;padding-top:80px;border:2px solid blue;">
                <form id="sign_in" method="POST">
                    <?php 
                    if(isset($_POST['forgetpassword'])){
                        $_SESSION['UETCL_USER_FORGET'] = "FORGOT";  
                        header("Location:".return_url());
                    }elseif(isset($_POST['login'])){
                        $username = $_POST['username'];
                        $password = $_POST['password'];
                        $db = new Db();


        //ini_set('display_errors', 1);

    
                       
                        $ad = new ActiveDirectory();
                        $ad_results = $ad->login($username, $password);

                        print_r($ad_results);   

                        $select = $db->select("SELECT * FROM sysuser WHERE user_name = '$username' AND user_password = '111$password'");
                        
                        if($db->num_rows()){
                            Feedback::success("Success Logged in. Please wait while redirecting");
                            Feedback::refresh();
                            extract($select[0][0]);
                            $_SESSION['UEDCL_USER_ID'] = $user_id;
                            $_SESSION['UEDCL_ROLE_ID'] = $user_id;

                            $update = $db->update("sysuser", ["user_last_logged_in"=>time(), "user_online"=>1], [user_id=>user_id()]);

                            AuditTrail::registerTrail("LOGIN-SUCCESSFULL", $db_id="",  "LOGIN-SUCCESSFULL", "LOGIN-SUCCESSFULL");
                            
                        }else{
                            Feedback::warning("Wrong User name or Password");

                            AuditTrail::registerTrail("LOGIN-FAILED", $db_id="",  "LOGIN-FAILED", "LOGIN-FAILED: username_entered->$username AND password->$password");
                        }
                    }

                     //AuditTrail::registerTrail("LOGIN - FAILED", $db_id="",  "LOGIN - SUCCESSFULL", "LOGIN - SUCCESSFULL");
                    ?>

                    <img src="<?php echo return_url().'images/uedcl.png'; ?>" style="position:absolute;top:-70px; left:30%; width:150px;"/>
                    <div class="sys_name"><span style="font-size:2em;color:blue;">UETCL</span><BR/>BILLING AND RECEIPTING SYSTEM</div>
                   
                    <div class="col-lg-12">
                        <div class="input-group">
                            <span class="input-group-addon">
                                <i class="fa fa-fx fa-user"></i>
                            </span>
                            <div class="form-line">
                                <input type="text" class="form-control" autocomplete="off" id="username" placeholder="Enter Username" autofocus>
                            </div>
                        </div>
                        <div class="input-group">
                            <span class="input-group-addon">
                                <i class="fa fa-fx fa-lock"></i>
                            </span>
                            <div class="form-line">
                                <input type="password" class="form-control" id="password" placeholder="Enter Password" >
                            </div>
                        </div>
                        <div class="clearfix"></div>
                    </div>

                    <div class="col-lg-3" style="display:none;">
                        <div class="msg">
                            <i class="fa fa-5x fa-unlock"></i><br/>
                        </div>
                    </div>
                    <div class="row">
                        <div class="col-xs-12">
                            <button id="loginBtn" class="btn btn-block bg-blue waves-effect" type="button">SIGN IN</button>
                            <span id="loginStatus"></span>
                        </div>
                    </div>                        
                    <div class="row m-t-15 m-b--20">
                        <div class="col-xs-12 align-right">
                          <!--  <button name="forgetpassword" value="GOOD" class="forgot-password">Forgot Password<i class="fa fa-fw fa-question-circle"></i></button> -->

                        </div>
                    </div>                    
                </form>
            </div>
        </div>
    </div>
    <?php }else {

    ?>
    <body class="theme-blue">

        <!-- Page Loader -->
    <div class="page-loader-wrapper">
        <div class="loader">
            <div class="preloader">
                <div class="spinner-layer pl-blue">
                    <div class="circle-clipper left">
                        <div class="circle"></div>
                    </div>
                    <div class="circle-clipper right">
                        <div class="circle"></div>
                    </div>
                </div>
            </div>
            <p>Please wait...</p>
        </div>
    </div>
    <script type="text/javascript">
        $(document).ready(function() {
            // Fade out the loader after 50 milliseconds
            setTimeout(function() {
                $('.page-loader-wrapper').fadeOut();
            }, 50);
            
            // Hide the top menu
            $('.toptopMenu').hide();
        });

        $(window).on('beforeunload', function() {
            setTimeout(function() {
                $('.page-loader-wrapper').fadeOut();
                $('.toptopMenu').show();
            }, 50);
        });
    </script>
    <?php 
    $show = array('formulae', 'schedule', 'wheeling-charge-schedule', 'invoice', 'readings');
    if(in_array(portion(2), $show) && portion(1) != 'optic-fibre-customers'){ ?>
    <script type="text/javascript">
        $('document').ready(function (){ 
            $('#leftsidebar').hide();  
            $('.content').css({'width':'95%', 'margin-left':'50px'});
            $('.toptopMenu').show();         
        });
    </script>
    <?php } ?>
    <!-- Overlay For Sidebars -->
    <div class="overlay"></div>
    <!-- #END# Overlay For Sidebars -->
    
    <!-- Top Bar -->
    <nav class="navbar">

        <div class="container-fluid" style="">
            <div class="navbar-header" style="margin-left:25px;padding-left:10px;" style="position:relative">
                <a href="javascript:void(0);" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar-collapse" aria-expanded="false"></a>
                <a href="javascript:void(0);" class="bars"><i class="fa fa-fx fa-ba"></i></a>
                <a class="navbar-brand" href="<?php display_url().''?>"><img src="<?php display_url();?>images/uedcl.png" alt="" style="position:absolute; top:5px; left:5px; height:inherit;"/> <span style="margin-left:50px;">UETCL BILLING AND RECEIPTING SYSTEM 
                <?php
                    $n = new BeforeAndAfter();
                    if(portion(2) != "help-energy"){
                        $current_customer = $n->rgf('customer', portion(3), 'customer_id', 'customer_short_name');
                        if($current_customer){
                            $type = ucwords(str_replace('-', ' ',portion(2)));
                            $year = portion(4);
                            $month = portion(5);
                            $sep = "/";
                        }

                        echo "<span class='text-dark' style='font-size:1em;padding:0 20px; font-weight:bold;color:black;'> $current_customer $type $month$sep$year</span>";
                    }
                ?>
                </span></a>                
            </div>
            <!-- <div class="menu-toggle">sdfsdf</div> -->
            <div class="collapse navbar-collapse" id="navbar-collapse">
                <ul class="nav navbar-nav navbar-right">
                    <li>
                    </li>
                    <li style="display:none;">
                        <a style="color:yellow"><div>You are logged in as :
                            
                            <?php 
                  //          $n = new BeforeAndAfter();

                            echo $n->full_name(user_id()); 
                            echo " (<b>". $n->rgf("designation", $n->rgf("sysuser", user_id(), "user_id", "user_designation"), "designation_id", "designation_name")."</b>) in <b>".$n->rgf("department", $n->rgf("sysuser", user_id(), "user_id", "user_department_id"), "dept_id", "dept_name")."</b>";

                            ?>
                            </div></a>
                    </li>
                    <li style="font-size: 12px; margin-top:2px;color:white; line-height: 16px;padding:0 20px;letter-spacing: 0.03em; color:black;">
                        <?php
                        $n = new BeforeAndAfter();
                         
                        echo 'Name: <b>'.$n->rgf("sysuser", user_id(), "user_id", "user_othername")." ".$n->rgf("sysuser", user_id(), "user_id", "user_surname") .'</b><br/>';
                        //echo 'Name: <b>'.$n->rgf("sysuser", user_id(), "user_id", "user_name").'</b><br/>';
                        echo 'User Role: <b>'.$n->rgf("designation", $n->rgf("sysuser", user_id(), "user_id", "user_designation"), "designation_id", "designation_name").'</b><br/>';
                        echo 'Department: <b>'.$n->rgf("department", $n->rgf("sysuser", user_id(), "user_id", "user_department_id"), "dept_id", "dept_name").'</b><br/>';
                            
                        
                        ?>
                    </li>
                   
                    <li>
                        <a href="<?php display_url(); ?>users/help">
                            <i class="fa fa-fx fa-question-circle text-danger"></i>
                            <span class="text-danger">Help</span>
                        </a>
                    </li>
                   
                    <li>
                        &nbsp;
                        &nbsp;
                        &nbsp;
                        &nbsp;
                        &nbsp;
                        
                    </li>
                   
                    <li>
                        <a onclick = "return confirm('Do you intend to logout ?');"href="<?php display_url(); ?>users/logout">
                            <i class="fa fa-fx fa-sign-out"></i>
                            <span>Logout</span>
                        </a>
                    </li>                   
                </ul>
            </div>
        </div>
            <div class="toptopMenu" style="background-color: black; width:100%; color:white;">
                <div class="" style="">
                    <nav class="menu_button">
                        <div></div>
                        <div></div>
                        <div></div>            
            
                        <div class="menu_top">
                            <ul class="top-menu">
                                <li><a href="<?php echo return_url(); ?>"><i class="fa fa-fw fa-home"></i> Home</a></li>
                                <li><a href="">
                                    <i class="fa fa-fw fa-users"></i> Energy Customers</a>
                                    <ul class="inner-top-menu">
                                        <li><a class="eagle-load" href="<?php echo return_url().'customers/add-customer'; ?>"><i class="fa fa-fw fa-plus"></i> Add Customer</a></li>
                                        <li><a class="eagle-load" href="<?php echo return_url().'customers/all-customers'; ?>"><i class="fa fa-fw fa-eye"></i> View Customer</a></li>
                                    </ul>
                                </li>
                                <li><a href="">
                                    <i class="fa fa-fw fa-users"></i> Optic Fibre Customers</a>
                                    <ul class="inner-top-menu">
                                        <li><a class="eagle-load" href="<?php echo return_url().'optic-fibre-customers/add-customer'; ?>"><i class="fa fa-fw fa-plus"></i> Add Customer</a></li>
                                        <li><a class="eagle-load" href="<?php echo return_url().'optic-fibre-customers/all-customers'; ?>"><i class="fa fa-fw fa-eye"></i> View Customer</a></li>
                                    </ul>
                                </li>
                                <li><a class="eagle-load" href="<?php echo return_url(); ?>services/uploaded-summary"><i class="fa fa-fw fa-list"></i> Readings Status</a></li>
                                <?php
                                    $g = Services::firstCustomer();
                                ?>
                                <li><a class="eagle-load" href="<?php echo return_url().'services/schedule/'.$g[0].'/'.$g[1].'/'.$g[2]; ?>"><i class="fa fa-fw fa-table"></i> Schedules</a></li>
                                <li><a class="eagle-load" href="<?php echo return_url(); ?>generators/add-generator"><i class="fa fa-fw fa-cogs"></i> Generators</a></li>
                            </ul>
                        </div>
                    </nav>
                </div>
            </div>
    </nav>
    <!-- #Top Bar -->
    <section>
        <!-- Left Sidebar -->
        <aside id="leftsidebar" class="sidebar">
                
            <div class="menu">
                <ul class="list">
                    
                    <li class="">
                        <a class="eagle-load" href="<?php display_url().''?>">
                            <i class="fa fa-fx fa-home"></i>
                            <span>Home</span>
                        </a>
                    </li>
                    <li <?php active('users'); ?>>
                        <a class="eagle-load" href="<?php echo return_url().'lock/all-readings-locks'; ?>">
                            <i class="fa fa-fx fa-lock"></i>
                            <span>Locking</span>
                        </a>
                    </li>
                    <li <?php active('users'); ?>>
                        <a class="eagle-load" href="<?php echo return_url().'audit-trail/view-audit-trail'; ?>">
                            <i class="fa fa-fx fa-circle"></i>
                            <span>Audit Trail</span>
                        </a>
                    </li>                    
                    <?php $active_list = array('template-upload','customers','add-boundary-node', 'uploaded-summary'); ?>
                    <li <?php active('customers'); if(in_array(portion(2), $active_list)) echo ' class="active"'; ?> >
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-plug"></i>
                            <span>Energy Sales</span>
                        </a>
                        <?php 
                        // $a = new AccessRights();
                        // echo $a->sectionAccess(user_id(), $link_page, $link_right);
                        ?>
                        <ul class="ml-menu">                           

                            <li class="" <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a href="<?php echo return_url().'services/template-upload'; ?>">
                                    <i class="fa fa-fx fa-upload"></i>
                                    <span>Upload Readings</span>
                                </a>
                            </li>
                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <?php
                                $g = Services::firstCustomer();
                                ?>
                                <a class="eagle-load" href="<?php echo return_url().'services/schedule/'.$g[0].'/'.$g[1].'/'.$g[2]; ?>">
                                    <i class="fa fa-fx fa-table"></i>
                                    <span>Schedules</span>
                                </a>
                            </li> 
                            <?php
                            foreach(Customers::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':' class="active" ';                                
                                    echo "<li "; 
                                    if(in_array(portion(2), $active_list)) echo ' class="active"';
                                    echo ">";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>

                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a class="eagle-load" href="<?php echo return_url().'services/uploaded-summary'; ?>">
                                    <i class="fa fa-fx fa-list"></i>
                                    <span>Readings Status</span>
                                </a>
                            </li>  

                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a class="eagle-load" href="<?php echo return_url().'other-charges/all-other-charges'; ?>">
                                    <i class="fa fa-fx fa-circle"></i>
                                    <span>Other Charges</span>
                                </a>
                            </li>
                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a class="eagle-load" href="<?php echo return_url().'kplc-tariff/all-tariff-schedule'; ?>">
                                    <i class="fa fa-fx fa-chain"></i>
                                    <span>Kpcl Tariff</span>
                                </a>
                            </li>
  
                            
                        </ul>
                    </li>
                    
                    
                    <li <?php active('other-invoice-customers'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-user"></i>
                            <span>Other Invoice Customers</span>
                        </a>
                        <?php 
                        // $a = new AccessRights();
                        // echo $a->sectionAccess(user_id(), $link_page, $link_right);
                        ?>
                        <ul class="ml-menu">
                            <?php
                            foreach(OtherInvoiceCustomers::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li> 
                    <li <?php active('other-invoices'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-file"></i>
                            <span>Other Invoices</span>
                            <?php  
                            $avr = new OtherInvoices();
                            if($avr->pending()||$avr->pending2()||$avr->pending3()||$avr->pending4()){  ?>
                            <div class="pull-right circle-number" style=""><?php echo $avr->pending()+$avr->pending2()+$avr->pending3()+$avr->pending4(); ?> </div>
                            <?php } ?>
                        </a>
                        <ul class="ml-menu">
                            <?php
                           foreach(OtherInvoices::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':' class="active" ';                                
                                    echo "<li $active>";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li> 
                    <li <?php active('efris-goods'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-list"></i>
                            <span>Efris Integration</span>
                            <?php  
                            $avr = new EfrisGoods();
                            if($avr->pending()){  ?>
                            <div class="pull-right circle-number" style=""><?php echo $avr->pending(); ?> </div>
                            <?php } ?>
                        </a>
                        <ul class="ml-menu">
                            <?php                           
                            foreach(EfrisGoods::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li> 
                    
                    <?php $active_list = array('add-generator','generator-bill-and-amount'); ?>
                    <li <?php active('generators'); ?><?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-money"></i>
                            <span>Energy Purchases</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(Generators::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':' class="active" ';                                
                                    echo "<li $active>";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                    <li <?php active('receipts'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-money"></i>
                            <span>Receipts</span>
                        </a>
                        <?php 
                        // $a = new AccessRights();
                        // echo $a->sectionAccess(user_id(), $link_page, $link_right);
                        ?>
                        <ul class="ml-menu">
                            <?php
                            foreach(Receipts::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li> 

                    <?php $active_list = array('all-expired-contracts','services', 'outlet', 'all-optic-fibre-invoices','add-customer','all-customers'); ?>
                    <?php  $avr = new OpticFibreCustomers(); ?>
                    <li <?php active('optic-fibre-customers'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-wifi"></i>
                            <span>Optic Fibre</span>
                            <?php 
                            if($avr->notifyAll()){
                                 echo '<div class="pull-right circle-number" style="">'.$avr->notifyAll().'</div>';
                                }
                            ?>
                        </a>
                        <ul class="ml-menu">
                              
                        <?php  $avr = new OpticFibreCustomers(); ?>                              
                        <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                            <a class="eagle-load" href="<?php echo return_url().'optic-fibre-customers/all-optic-fibre-invoices'; ?>">
                                <i class="fa fa-fx fa-table"></i>
                                <span>Generated Invoices</span>
                                <?php
                                if($avr->notifyAll()){
                                 echo '<div class="pull-right circle-number" style="background-color:black;">'.$avr->notifyAll().'</div>';
                                }
                                ?>
                            </a>
                        </li> 

                            <?php
                           
                            foreach(OpticFibreCustomers::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':' class="active" ';                                
                                    echo "<li ";
                                    if(in_array(portion(2), $active_list)) echo ' class="active"';
                                    echo ">";
                                    echo '<a class="eagle-load" href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }

                            ?>

                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a class="eagle-load" href="<?php echo return_url().'optic-fibre-customers/services'; ?>">
                                    <i class="fa fa-fx fa-cogs"></i>
                                    <span>Services</span>
                                </a>
                            </li>

                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a class="eagle-load" href="<?php echo return_url().'optic-fibre-customers/outlet'; ?>">
                                    <i class="fa fa-fx fa-cogs"></i>
                                    <span>Outlets</span>
                                </a>
                            </li>
                            <?php
                            $v = new OpticFibreCustomers();
                            
                            if(count($v->expiredContracts())){
                            ?>
                            <li <?php if(in_array(portion(2), $active_list)) echo ' class="active"'; ?>>
                                <a class="eagle-load" href="<?php echo return_url().'optic-fibre-customers/all-expired-contracts'; ?>">
                                    <i class="fa fa-fx fa-circle"></i>
                                    <span>Expired Contracts</span>
                                    <?php
                                    if(count($v->expiredContracts())){
                                     echo '<div class="pull-right circle-number" style="">'.count($v->expiredContracts()).'</div>';
                                    }
                                    ?>
                                </a>
                            </li>                
                            <?php } ?>                            

                        </ul>
                    </li>                 
                          
                    <li style="display:none;"<?php // active('audit-trail'); ?>>
                        <a href="<?php echo return_url().'accounts/account-statement'; ?>">
                            <i class="fa fa-fx fa-table"></i>
                            <span>Account Statement</span>
                        </a>
                    </li>        
                    <li style="display:none;"<?php // active('audit-trail'); ?>>
                        <a href="<?php echo return_url().'services/view-invoice'; ?>">
                            <i class="fa fa-fx fa-table"></i>
                            <span>Invoices - Energy</span>
                        </a>
                    </li>

                        
                  
                        <?php  $avr = new OpticFibreCustomers(); ?>  
                    <li <?php active('optic-fibre-customers'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-thumbs-o-up"></i>
                            <span>Approvals - Optic Fibre</span>
                           <?php
                            if($avr->notifyAll() && user_id() == static_generator2()){
                                echo '<div class="pull-right circle-number" style="">'.$avr->notifyAll().'</div>';
                            }else{
                                if($avr->toApprove()){
                                    echo '<div class="pull-right circle-number" style="">'.$avr->toApprove().'</div>';
                                }
                            }
                            ?>
                        </a>
                        <ul class="ml-menu">
                            <li><a href="<?php echo return_url().'optic-fibre-customers/approve-optic-fibre-invoices'; ?>"> Approved</a></li>
                            <li><a href="<?php echo return_url().'optic-fibre-customers/pending-approval-optic-fibre-invoices'; ?>"> Pending</a></li>
                        </ul>
                    </li>
                                                           
                    <li <?php  active('customers'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-thumbs-o-up"></i>
                            <span>Approvals - Energy</span>
                            <?php
                            $v = new Services();
                            if(!empty($v->isOnListAndIsPending())){
                             echo '<div class="pull-right circle-number" style="">'.($v->isOnListAndIsPending()).'</div>';
                            }
                            ?>
                        </a>
                        <ul class="ml-menu">
                            <li><a href="<?php echo return_url().'services/energy-approval-list'; ?>"> Approved</a></li>
                            <li><a href="<?php echo return_url().'services/energy-approval-list-pending'; ?>"> Pending</a></li>
                        </ul>
                    </li>
                     <li <?php active('reports');
                     ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-list"></i>
                            <span>Reports</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(Reports::getLinks() as $link){
                                extract($link);
                                $x = end(explode('/', $link_address));                              
                                $active = ($x == portion(2))? ' class="active" ':"";                                
                                echo "<li $active>";
                                echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                echo '</li>';
                            }
                            ?>
                        </ul>
                    </li> 
                    <li <?php active('users'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-users"></i>
                            <span>Users</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(Users::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                                      
                    <li <?php active('access-rights'); ?>>
                        <a href="<?php display_url();?>access-rights/all-user-rights-and-privileges">
                            <i class="fa fa-fx fa-building"></i>
                            <span>User Rights & Privileges</span>
                        </a>
                        
                    </li>
                    <li <?php active('designation'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-user"></i>
                            <span>User Role</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(Designation::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                
                
                    <li <?php active('department'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-building"></i>
                            <span>Departments</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(Department::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                     <li <?php active('department'); ?> style="">
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-chain"></i>
                            <span>Journals</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(JournalMapping::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                    <li <?php active('jv-approval-levels'); ?> style="">
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-align-center"></i>
                            <span>JV Approval Levels</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(JVApprovalLevels::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                    <li <?php active('department'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
                            <i class="fa fa-fx fa-briefcase"></i>
                            <span>Dictionary</span>
                        </a>
                        <ul class="ml-menu">
                            <?php
                            foreach(Dictionary::getLinks() as $link){
                                extract($link);
                                $a = new AccessRights();
                                if($a->sectionAccess(user_id(), $link_page, $link_right)){
                                    $x = end(explode('/', $link_address));                              
                                    $active = ($x == portion(2))? ' class="active" ':"";                                
                                    echo "<li $active>";
                                    echo '<a href="'.return_url().$link_address.'"><i class="fa fa-fx '.$link_icon.'"></i> '.$link_name.'</a>';
                                    echo '</li>';
                                }
                            }
                            ?>
                        </ul>
                    </li>
                    
                    

                    <li>
                        <a href="<?php display_url(); ?>users/changed-password">
                            <i class="fa fa-fx fa-cogs"></i>
                            <span>User Account Settings</span>
                        </a>
                    </li>
                    
                    <li>
                        <a onclick = "return confirm('Do you intend to logout ?');" href="<?php display_url(); ?>users/logout">
                            <i class="fa fa-fx fa-sign-out"></i>
                            <span>Logout</span>
                        </a>
                    </li>
                    <li style="margin:40px 0;"></li>
                </ul>
                
            </div>
            
            <!-- #Menu -->
            <!-- Footer -->
            <div class="legal">
                <div class="copyright">
                    <?php 
        echo strtotime('2024-07-15');
        ?>
                    &copy; 2017 - <?php echo date('Y');?> <a href="javascript:void(0);">FLAXEM</a>.
                </div>
                <div class="version">
                    <b>Version: </b> 6.0
                </div>
            </div>
            <!-- #Footer -->
        </aside>
        <!-- #END# Left Sidebar -->
    </section>

    <section class="content">
        <div class="container-fluid" id="EagleContainer">
            <?php 
            include "Admin_functioncalls.php";
            echo '<div class="clearfix"></div>';

            ?>         
            <div class="clearfix"></div>
            <br/><br/>
        </div>
    </section>
    <?php } ?>
    <script type="text/javascript" src="<?php display_url();?>js/number.js"></script>
    <script src="<?php display_url(); ?>css/bootstrap/js/bootstrap.js"></script>
    <!-- Select Plugin Js -->
    <script src="<?php display_url(); ?>css/bootstrap-select/js/bootstrap-select.js"></script>

    <!-- Slimscroll Plugin Js -->
    <script src="<?php display_url(); ?>css/jquery-slimscroll/jquery.slimscroll.js"></script>

    <!-- Waves Effect Plugin Js -->
    <script src="<?php display_url(); ?>css/node-waves/waves.js"></script>

    <!-- Jquery CountTo Plugin Js -->
    <script src="<?php display_url(); ?>css/jquery-countto/jquery.countTo.js"></script>

    <!-- Morris Plugin Js -->
    <script src="<?php display_url(); ?>css/raphael/raphael.min.js"></script>
    <script src="<?php display_url(); ?>css/morrisjs/morris.js"></script>

    <!-- ChartJs -->
    <script src="<?php display_url(); ?>css/chartjs/Chart.bundle.js"></script>

    <!-- Flot Charts Plugin Js -->
    <script src="<?php display_url(); ?>css/flot-charts/jquery.flot.js"></script>
    <script src="<?php display_url(); ?>css/flot-charts/jquery.flot.resize.js"></script>
    <script src="<?php display_url(); ?>css/flot-charts/jquery.flot.pie.js"></script>
    <script src="<?php display_url(); ?>css/flot-charts/jquery.flot.categories.js"></script>
    <script src="<?php display_url(); ?>css/flot-charts/jquery.flot.time.js"></script>

    <!-- Waves Effect Plugin Js -->
    <script src="<?php display_url(); ?>css/node-waves/waves.js"></script>
    
    <!-- Autosize Plugin Js -->
    <script src="<?php display_url(); ?>css/plugins/autosize/autosize.js"></script>

    
    
    <!-- Custom Js -->
    <script src="<?php display_url(); ?>js/admin.js"></script>
    <script src="<?php display_url(); ?>js/pages/index.js"></script>
    <script src="<?php display_url(); ?>js/pages/forms/basic-form-elements.js"></script>
    
    <!-- Demo Js -->
    <script src="<?php display_url(); ?>js/demo.js"></script>

    <script type="text/javascript" src="<?php display_url(); ?>js/jquery.tablesorter.js"></script>
    <script type="text/javascript" src="<?php display_url(); ?>js/jquery.tablesorter.widgets.js"></script>
 
    <script type="text/javascript" src="<?php display_url(); ?>DataTables/datatables.min.js"></script>
    <script type="text/javascript">
        $(function() {
            // $.tablesorter.addWidget({
            //     id: "numbering",
            //     format: function(table) {
            //         var c = table.config;
            //         $("tr:visible", table.tBodies[0]).each(function(i) {
            //             $(this).find('td').eq(0).text(i + 1);
            //         });
            //     }
            // });

          $("#sortSpan").tablesorter({
            headers: {
                sorter: false
            },
            theme : 'blue',

    widgets : [ 'zebra','stickyHeaders', 'numbering'],
    widgetOptions : {
      filter_external: 'input.search',
      filter_reset: '.reset',
      // extra class name added to the sticky header row
      stickyHeaders : '',
      // number or jquery selector targeting the position:fixed element
      stickyHeaders_offset : 0,
      // added to table ID, if it exists
      stickyHeaders_cloneId : '-sticky',
      // trigger "resize" event on headers
      stickyHeaders_addResizeEvent : true,
      // if false and a caption exist, it won't be included in the sticky header
      stickyHeaders_includeCaption : true,
      // The zIndex of the stickyHeaders, allows the user to adjust this to their needs
      stickyHeaders_zIndex : 2,
      // jQuery selector or object to attach sticky header to
      stickyHeaders_attachTo : null,
      // jQuery selector or object to monitor horizontal scroll position (defaults: xScroll > attachTo > window)
      stickyHeaders_xScroll : null,
      // jQuery selector or object to monitor vertical scroll position (defaults: yScroll > attachTo > window)
      stickyHeaders_yScroll : null,

      // scroll table top into view after filtering
      stickyHeaders_filteredToTop: true,

      scroller_fixedColumns : 2,

    }
          });
        });
        $(document).ready( function () {
            $('#table').DataTable({
                "iDisplayLength": 15
            });
        } );

        $('#input_starttime').pickatime({
        // 12 or 24 hour
        twelvehour: true,
        });

        function print(x){
            if(confirm("Do you really want to print")){                 
                var printing = window.open('','','left=0,top=0,width=700,height=400,toolbar=0,scrollbars=0,status=0');
                printing.document.write(document.getElementById('printMe').innerHTML);
                printing.document.close();
                printing.focus();
                printing.print();
                printing.close();
            }else{
                return false;
            }
        }
    </script>
    <script>
    $('.select2').select2();
</script>
</body>

</html>
