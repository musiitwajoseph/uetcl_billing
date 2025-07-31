<?php
include "../classes/Db.php";
include "../classes/Field_calculate.inc";
include "../classes/BeforeAndAfter.inc";
include "../classes/FeedBack.php";
include "../classes/AuditTrail.inc";
include "../classes/AccessRights.inc";
include "../classes/Efris.inc";
include ("../qrcode/qrlib.php");

$t = new BeforeAndAfter();

$name = $_POST['name'];
$telephone = $_POST['telephone'];
$description = $_POST['description'];
$amount = $_POST['amount'];
$receiptNo = $_POST['receiptNo'];

// echo "Dear $name, UETCL has confirmed receiving amount: $amount Being payment for $description. Receipt No. $receiptNo Thnx".$telephone;


sms("Dear $name, \nUETCL has confirmed receiving amount: $amount Being payment for $description. Receipt No. $receiptNo.\n Thnx", $telephone);

// Feedback::sms("Your Loan payment of: ".($amount)." has been received. Thnx", '256788229210');


//Feedback::sms("Your Loan payment of: ".($amount)." has been received. Thnx", '256759617100');



function sms($message, $number="+256788229210"){
		//return true;
		// Be sure to include the file you've just downloaded
		require_once('../SMS/AfricasTalkingGateway.php');
		// Specify your login credentials
		$username   = "musiitwa";
		$apikey     = "a9cb69df39cecf8d58f4ce2d69d4e6c6b9dd1250196298e1533bed74ec3733d6";
		// NOTE: If connecting to the sandbox, please use your sandbox login credentials
		// Specify the numbers that you want to send to in a comma-separated list
		// Please ensure you include the country code (+256 for Uganda in this case)


		$nums = array($number);

		{

		$recipients = "".implode(',', $nums);
		// And of course we want our recipients to know what we really do
		$message = "".$message;
		
		// Create a new instance of our awesome gateway class
		$gateway    = new AfricasTalkingGateway($username, $apikey);
		// NOTE: If connecting to the sandbox, please add the sandbox flag to the constructor:
		/*************************************************************************************
					 ****SANDBOX****
		$gateway    = new AfricasTalkingGateway($username, $apiKey, "sandbox");
		**************************************************************************************/
		// Any gateway error will be captured by our custom Exception class below, 
		// so wrap the call in a try-catch block
		try 
		{ 
		  // Thats it, hit send and we'll take care of the rest. 
		  $results = $gateway->sendMessage($recipients, $message);
					
		  foreach($results as $result) {
			// status is either "Success" or "error message"
			//echo " Number: " .$result->number;
			//echo " Status: " .$result->status;
			//echo " MessageId: " .$result->messageId;
			//echo " Cost: "   .$result->cost."\n\n";
			echo '';
		  }
		}
		catch ( AfricasTalkingGatewayException $e )
		{
		 // @echo "Encountered an error while sending: ".$e->getMessage();
		}

		}
	}