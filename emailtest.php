<?php
error_reporting(1);
ini_set('display_errors', 1);
//================= TO ==================
$to = "joseph.musiitwa@uetcl.com";
$subject = "EMAIL WORKING NOW";
$message = "CODE DESCRIPTION - VIOLA, JOSEPH, CHILLION WORKED TO NIGHT";


//================== MAIL SERVER =====================
	/*
define('EMAIL_SMTP_DEBUG', '2');   				//2
define('EMAIL_SERVER', 'smtp.gmail.com');		//HOST
define('EMAIL_SMTP_AUTH', TRUE);		
define('EMAIL_PORT', 587);
define('EMAIL_USERNAME', 'josemusiitwa@gmail.com');
define('EMAIL_PASSWORD', 'zdnydizegtxwwbaw');
define('EMAIL_SMTP_SECURE', 'tls');
define('EMAIL_SET_FROM', 'fse@flaxem.com');*/
//=====================================================
//================== MAIL SERVER =====================
	
define('EMAIL_SMTP_DEBUG', '1');   				//2
define('EMAIL_SERVER', 'uetcl-com.mail.protection.outlook.com');	//HOST
define('EMAIL_SMTP_AUTH', FALSE);		
define('EMAIL_PORT', 25);
define('EMAIL_USERNAME', 'info@uetcl.com');
define('EMAIL_PASSWORD', '1616DD????');
define('EMAIL_SMTP_SECURE', 'tls');
define('EMAIL_SET_FROM', 'info@uetcl.com');
//=====================================================


 function sendmailz($to,$subject,$message,$name, $telephone="+256788229210", $attachments = array()){
	
	$subject = $subject;
	/*if(!isInternetOn()){
		return 0;
		FeedBack::warning("Email not sent, No Internet, Try checking the Network cables, Modem or Router");
		return 0;
	}*/
	require_once('PHPMailer-master/src/PHPMailer.php');
	require_once('PHPMailer-master/src/SMTP.php');
	$mail             = new PHPMailer();
	$body             = $message;
	$mail->IsSMTP();

	if(EMAIL_SMTP_DEBUG){
		$mail->SMTPDebug = EMAIL_SMTP_DEBUG;
	}


	$mail->SMTPOptions = array(
		'ssl' => array(
			'verify_peer' => false,
			'verify_peer_name' => false,
			'allow_self_signed' => false
		)
	);

	/////////////////////////////////////////////////
	
	$mail->Host       = EMAIL_SERVER;                  
	$mail->SMTPAuth   = EMAIL_SMTP_AUTH;
	$mail->Port       = EMAIL_PORT;
	$mail->Username   = EMAIL_USERNAME;
	$mail->Password   = EMAIL_PASSWORD;
	$mail->SMTPSecure = EMAIL_SMTP_SECURE;

	/////////////////////////////////////////////////
	$mail->SetFrom(EMAIL_SET_FROM, 'info@uetcl.com');
	$mail->AddReplyTo(EMAIL_SET_FROM,"info@uetcl.com");
	
	if(count($attachments)){
		foreach($attachments as $attach_file){
			$mail->AddAttachment(str_replace(return_url(),'',$attach_file));
		}
	}
	$mail->Subject    = $subject;
	$mail->AltBody    = "To view the message, please use an HTML compatible email viewer!";
	$mail->MsgHTML($body);
	//$address = $to;
	foreach($to as $address){
		$mail->AddAddress($address, $name);
	}
	if(!$mail->Send()) {
	  //echo "Failed ".$mail->ErrorInfo;
	  return 0;
	} else {
		//echo "Sent ";
		return 1;
	}
}


sendmailz(array($to), $subject,$message,"TEST", "TEST");