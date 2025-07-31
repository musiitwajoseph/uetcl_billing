<!DOCTYPE html>
<html>
<?php

function error($error_array) {
    echo '<div id="error">';
    echo "Please review the following fields:<br />";
    echo '<div style="padding:2px 20px;">';
    foreach($error_array as $error) {
        echo " - " . $error . "<br />";
    }
    echo '</div>';
    echo "</div>";
}
function refresh($seconds = 3, $link = ""){
    if($link == ""){
        echo '<meta http-equiv="refresh" content="'.$seconds.'"/>';
    }else{
        echo '<meta http-equiv="refresh" content="'.$seconds.';'.$link.'"/>';
    }
}
function returnFieldFromCustomer($id, $toReturn){
    global $con;
    $select = mysqli_query($con, "select $toReturn from customers where cus_id = $id");
    $row = mysqli_fetch_assoc($select);
    return $row[$toReturn];
}

function returnFieldFromService($id, $toReturn){
    global $con;
    $select = mysqli_query($con, "select $toReturn from service where se_id = $id");
    $row = mysqli_fetch_assoc($select);
    return $row[$toReturn];
}

function month_name($num){
    $monthNum  = $num;
    $dateObj   = DateTime::createFromFormat('!m', $monthNum);
    return $monthName = $dateObj->format('F'); 
}
function rate_calculate($cus_id, $date="", $metering_point=""){
    global $con;
    if(empty($date)){
        //sdafaf
        $sel = mysqli_query($con, "SELECT rea_id, rea_date FROM r_reading WHERE rea_cus_id = $cus_id AND rea_mp_id = '$metering_point' ORDER BY rea_date DESC LIMIT 1");
    }else{
        $year = date('Y', @$date);
        $month = date('m', @$date);
        $sel = mysqli_query($con, "SELECT rea_id, rea_date FROM r_reading WHERE year(from_unixtime(rea_date)) = '$year' AND month(from_unixtime(rea_date)) = $month AND rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point' lIMIT 1");
    }
    
    $ro = mysqli_fetch_assoc($sel);
    @extract($ro);
    //selecting rates
    //echo "SELECT rate_import_wh_1 as rat1, rate_import_wh_2 as rat2, rate_import_wh_3 as rat3, rate_cus_id FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id' LIMIT 1";
    $sel = mysqli_query($con, "SELECT rate_import_wh_1 as rat1, rate_import_wh_2 as rat2, rate_import_wh_3 as rat3, rate_cus_id FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id' LIMIT 1");
    $ro = mysqli_fetch_assoc($sel);
    @extract($ro);

    $rea_date = date('m', ($rea_date));

    $sel = mysqli_query($con, "SELECT rea_id FROM r_reading WHERE rea_cus_id = $cus_id  AND rea_mp_id = '$metering_point' AND month(from_unixtime(rea_date)) < $rea_date ORDER BY rea_date DESC LIMIT 1");  
    $ro = mysqli_fetch_assoc($sel);
    @extract($ro);
    //echo $rea_date;

    if(mysqli_num_rows($sel)){      
        //selecting rates
        $sel = mysqli_query($con, "SELECT rate_import_wh_1 as rate1, rate_import_wh_2 as rate2, rate_import_wh_3 as rate3, rate_cus_id FROM r_rate WHERE rate_reading_id = '$rea_id' AND rate_cus_id = '$cus_id' lIMIT 1");
        $ro = mysqli_fetch_assoc($sel);
        @extract($ro);  
    }
        
    $r1 = ceil(@$rat1 - @$rate1);
    $r2 = ceil(@$rat2 - @$rate2);
    $r3 = ceil(@$rat3 - @$rate3);
    
    $to_return = array($r1, $r2,$r3, ($r1+$r2+$r3));
    
    return $to_return;
    
}
function number_to_words($num){
        $num    = ( string ) ( ( int ) $num );
       
        if( ( int ) ( $num ) && ctype_digit( $num ) )
        {
            $words  = array( );
           
            $num    = str_replace( array( ',' , ' ' ) , '' , trim( $num ) );
           
            $list1  = array('','one','two','three','four','five','six','seven',
                'eight','nine','ten','eleven','twelve','thirteen','fourteen',
                'fifteen','sixteen','seventeen','eighteen','nineteen');
           
            $list2  = array('','ten','twenty','thirty','forty','fifty','sixty',
                'seventy','eighty','ninety','hundred');
           
            $list3  = array('','thousand','million','billion','trillion',
                'quadrillion','quintillion','sextillion','septillion',
                'octillion','nonillion','decillion','undecillion',
                'duodecillion','tredecillion','quattuordecillion',
                'quindecillion','sexdecillion','septendecillion',
                'octodecillion','novemdecillion','vigintillion');
           
            $num_length = strlen( $num );
            $levels = ( int ) ( ( $num_length + 2 ) / 3 );
            $max_length = $levels * 3;
            $num    = substr( '00'.$num , -$max_length );
            $num_levels = str_split( $num , 3 );
           
            foreach( $num_levels as $num_part )
            {
                $levels--;
                $hundreds   = ( int ) ( $num_part / 100 );
                $hundreds   = ( $hundreds ? ' ' . $list1[$hundreds] . ' Hundred' . ( $hundreds == 1 ? '' : '' ) . ' ' : '' );
                $tens       = ( int ) ( $num_part % 100 );
                $singles    = '';
               
                if( $tens < 20 )
                {
                    $tens   = ( $tens ? ' ' . $list1[$tens] . ' ' : '' );
                }
                else
                {
                    $tens   = ( int ) ( $tens / 10 );
                    $tens   = ' ' . $list2[$tens] . ' ';
                    $singles    = ( int ) ( $num_part % 10 );
                    $singles    = ' ' . $list1[$singles] . ' ';
                }
                $words[]    = $hundreds . $tens . $singles . ( ( $levels && ( int ) ( $num_part ) ) ? ' ' . $list3[$levels] . ' ' : '' );
            }
           
            $commas = count( $words );
           
            if( $commas > 1 )
            {
                $commas = $commas - 1;
            }
           
            $words  = implode( ', ' , $words );
           
            //Some Finishing Touch
            //Replacing multiples of spaces with one space
            $words  = trim( str_replace( ' ,' , ',' , trim_all( ucwords( $words ) ) ) , ', ' );
            if( $commas )
            {
                $words  = str_replace_last( ',' , ' ' , $words );
            }
           
            return $words;
        }
        else if( ! ( ( int ) $num ) )
        {
            return 'Zero';
        }
        return '';
    }
   
include "classes/init.inc";
///////////////////////////////////////////////////
$con = mysqli_connect("localhost","root","", "uetcl_db");
if(!$con) echo 'No selecting'.mysqli_error($con);
//////////////////////////////////////////////////
?>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=Edge">
    <meta content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" name="viewport">
    <title>UETCL BILLING AND RECIEPT MANAGEMENT SYSTEM</title>
    <!-- Favicon-->
    <link rel="icon" href="favicon.ico" type="image/x-icon">
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

    <!-- Custom Css -->
    <link href="<?php display_url(); ?>css/style.css" rel="stylesheet">

    <!-- AdminBSB Themes. You can choose a theme from css/themes instead of get all themes -->
    <link href="<?php display_url(); ?>css/themes/all-themes.css" rel="stylesheet" />
    
    <script type="text/javascript" src="<?php display_url();?>js/script.js"></script>

 <!-- Jquery Core Js -->
    <script type="text/javascript" charset="utf8" src="<?php display_url(); ?>jquery-3.4.1.min.js"></script>

    <!-- Bootstrap Core Js -->
    <script src="<?php display_url(); ?>css/bootstrap/js/bootstrap.js"></script>

    <script type="text/javascript" src="<?php display_url();?>css/bootstrap-material-datetimepicker/js/material.min.js"></script>
    
    <script type="text/javascript" src="<?php display_url();?>css/bootstrap-material-datetimepicker/js/moment-with-locales.min.js"></script>

    <script type="text/javascript" src="<?php display_url();?>css/bootstrap-material-datetimepicker/js/bootstrap-material-datetimepicker.js"></script>

</head>

    <?php
    if(isset($_SESSION['UEDCL_USER_FORGET'])){
    ?>
    <body class="login-page">
    <div class="login-box"><br/><br/><br/>       
        <div class="card">
            <div class="body" style="position:relative;margin-top:60px;border-radius:20px;padding-top:80px;border:2px solid blue;">
                <form id="sign_in" method="POST"> 
                    <?php 
                    if(isset($_POST['passwordremembered'])){                       
                        unset($_SESSION['UEDCL_USER_FORGET']);
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
                                $subject = "UEDCL FORGOT PASSWORD";
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
                    <div class="sys_name"><span style="font-size:2em;color:blue;">UEDCL</span><BR/>BILLING AND RECIEPTING SYSTEM</div>

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
    <body class="login-page">
    <div class="login-box"><br/><br/><br/>       
        <div class="card">
            <div class="body" style="position:relative;margin-top:60px;border-radius:20px;padding-top:80px;border:2px solid blue;">
                <form id="sign_in" method="POST">
                    <?php 
                    if(isset($_POST['forgetpassword'])){
                        $_SESSION['UEDCL_USER_FORGET'] = "FORGOT";  
                        header("Location:".return_url());
                    }elseif(isset($_POST['login'])){
                        $username = $_POST['username'];
                        $password = $_POST['password'];
                        $db = new Db();
                        $select = $db->select("SELECT * FROM sysuser WHERE user_name = '$username' AND user_password = '$password'");
                        
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
                    <div class="sys_name"><span style="font-size:2em;color:blue;">UETCL</span><BR/>BILLING AND RECIEPTING SYSTEM</div>
                   
                    <div class="col-lg-12">
                        <div class="input-group">
                            <span class="input-group-addon">
                                <i class="fa fa-fx fa-user"></i>
                            </span>
                            <div class="form-line">
                                <input type="text" class="form-control" name="username" placeholder="Enter Username" autofocus>
                            </div>
                        </div>
                        <div class="input-group">
                            <span class="input-group-addon">
                                <i class="fa fa-fx fa-lock"></i>
                            </span>
                            <div class="form-line">
                                <input type="password" class="form-control" name="password" placeholder="Enter Password" >
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
                            <button name="login" class="btn btn-block bg-blue waves-effect" type="submit">SIGN IN</button>
                        </div>
                    </div>                        
                    <div class="row m-t-15 m-b--20">
                        <div class="col-xs-12 align-right">
                           <button name="forgetpassword" value="GOOD" class="forgot-password">Forgot Password<i class="fa fa-fw fa-question-circle"></i></button>

                        </div>
                    </div>                    
                </form>
            </div>
        </div>
    </div>
    <?php }else {
    ?>
	<body class="theme-blue">
    <!-- Overlay For Sidebars -->
    <div class="overlay"></div>
    <!-- #END# Overlay For Sidebars -->
    
    <!-- Top Bar -->
    <nav class="navbar">
        <div class="container-fluid">
            <div class="navbar-header" style="margin-left:25px;padding-left:10px;" style="position:relateive">
                <a href="javascript:void(0);" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar-collapse" aria-expanded="false"></a>
                <a href="javascript:void(0);" class="bars"><i class="fa fa-fx fa-ba"></i></a>
                <a class="navbar-brand" href="<?php display_url().''?>"><img src="<?php display_url();?>images/uedcl.png" alt="" style="position:absolute; top:5px; left:5px; height:inherit;"/> <span style="margin-left:50px;">UETCL BILLING AND RECIEPTING SYSTEM</span></a>                
            </div>

            <div class="collapse navbar-collapse" id="navbar-collapse">
                <ul class="nav navbar-nav navbar-right">
                    <li>
                    </li>
                    <li style="display:none;">
                        <a style="color:yellow"><div>You are logged in as :
                            
                            <?php 
                            $n = new BeforeAndAfter();

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
                        <a onclick = "return confirm('Do you intend to logout ?');"href="<?php display_url(); ?>users/logout">
                            <i class="fa fa-fx fa-sign-out"></i>
                            <span>Logout</span>
                        </a>
                    </li>                    
                </ul>
            </div>
        </div>
    </nav>
    <!-- #Top Bar -->
    <section>
        <!-- Left Sidebar -->
        <aside id="leftsidebar" class="sidebar">
            <!-- User Info -->
			<!--
            <div class="user-info">
                <div class="image">
                    <img src="images/user.png" width="48" height="48" alt="User" />
                </div>
                <div class="info-container">
                    <div class="name" data-toggle="dropdown" aria-haspopup="true" aria-expanded="false">John Doe</div>
                    <div class="email">john.doe@example.com</div>
                    <div class="btn-group user-helper-dropdown">
                        <i class="material-icons" data-toggle="dropdown" aria-haspopup="true" aria-expanded="true">keyboard_arrow_down</i>
                        <ul class="dropdown-menu pull-right">
                            <li><a href="javascript:void(0);"><i class="material-icons">person</i>Profile</a></li>
                            <li role="separator" class="divider"></li>
                            <li><a href="javascript:void(0);"><i class="material-icons">group</i>Followers</a></li>
                            <li><a href="javascript:void(0);"><i class="material-icons">shopping_cart</i>Sales</a></li>
                            <li><a href="javascript:void(0);"><i class="material-icons">favorite</i>Likes</a></li>
                            <li role="separator" class="divider"></li>
                            <li><a href="javascript:void(0);"><i class="material-icons">input</i>Sign Out</a></li>
                        </ul>
                    </div>
                </div>
            </div>-->
            <!-- #User Info -->
            <!-- Menu -->
			
            <div class="menu">
                <ul class="list">
                    <li class="">
                        <a href="<?php display_url().''?>">
                            <i class="fa fa-fx fa-home"></i>
                            <span>Home</span>
                        </a>
                    </li>
                    <li <?php active('audit-trail'); ?>>
                        <a href="<?php echo return_url().'audit-trail/view-audit-trail'; ?>">
                            <i class="fa fa-fx fa-circle"></i>
                            <span>Audit Trail</span>
                        </a>
                    </li>
                    <li <?php // active('audit-trail'); ?>>
                        <a href="<?php echo return_url().'services/add-customer-service'; ?>">
                            <i class="fa fa-fx fa-users"></i>
                            <span>Add Services</span>
                        </a>
                    </li> 

                    <li <?php // active('audit-trail'); ?>>
                        <a href="">
                            <i class="fa fa-fx fa-circle"></i>
                            <span>Customers</span>
                        </a>
                    </li>                    
                           
                    <li <?php // active('audit-trail'); ?>>
                        <a href="<?php echo return_url().'services/import'; ?>">
                            <i class="fa fa-fx fa-circle"></i>
                            <span>Import Readings</span>
                        </a>
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
                            <span>Deptments</span>
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
					<li <?php active('branches'); ?>>
                        <a href="javascript:void(0);" class="menu-toggle">
							<i class="fa fa-fx fa-tree"></i>
                            <span>Office</span>
                        </a>
                        <ul class="ml-menu">
							<?php
							foreach(Branches::getLinks() as $link){
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
                    
                    <li <?php 
                    //active('reports');
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
                    &copy; <?php echo date('Y');?> <a href="javascript:void(0);">FLAXEM</a>.
                </div>
                <div class="version">
                    <b>Version: </b> 1.0
                </div>
            </div>
            <!-- #Footer -->
        </aside>
        <!-- #END# Left Sidebar -->
    </section>

    <section class="content">
        <div class="container-fluid">
			<?php 
           
			include "Admin_functioncalls.php";
			?>         
            <div class="clearfix"></div>
            <br/><br/>
        </div>
    </section>
	<?php } ?>
   

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
 
	<script type="text/javascript" src="<?php display_url(); ?>DataTables/datatables.min.js"></script>
	<script type="text/javascript">
		$(document).ready( function () {
			$('#table').DataTable({
                "iDisplayLength": 15
            });
		} );

        $('#input_starttime').pickatime({
        // 12 or 24 hour
        twelvehour: true,
        });

	</script>
</body>

</html>
