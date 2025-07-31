<?php 
Class FeedBack{
	
	function errors($error_array) {
		echo '<div class="alert alert-danger"><button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>';
		echo "Please review the following fields:<br />";
		foreach($error_array as $error) {
			echo " - " . $error . "<br />";
		}
		echo "</div>";
	}
	function error($string="") {
		echo '<div class="alert alert-danger"><button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>';	
		echo $string;
		echo "</div>";
	}

	function success($string="Successfully saved"){
		echo '<div class="alert alert-success"><button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>'.$string.'</div>';
	}

	function warning($string=""){
		echo '<div class="alert alert-warning"><button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>'.$string.'</div>';
	}
	
	function refresh($seconds = 3, $link = ""){
		if($link == ""){
			echo '<meta http-equiv="refresh" content="'.$seconds.'"/>';
		}else{
			echo '<meta http-equiv="refresh" content="'.$seconds.';'.$link.'"/>';
		}
	}
	
	function redirect($url){
		header("Loction:$url");
	}
	
	function status($start_date, $end_date){
		$time = time();
		if($start_date <= $time && $end_date >= $time){
				return '<button class="btn btn-xs btn-success"><i class="fa fa-fw fa-check-circle"></i>Event Started </button>';
		}elseif($start_date >= $time && $end_date >= $time){
			return '<button class="btn btn-xs btn-danger"><i class="fa fa-fw fa-times"></i> Not Active </button>';
		}else{
			return '<button class="btn btn-xs btn-danger"><i class="fa fa-fw  fa-times"></i>Event Closed</button>';
		}		
	}
	
	function date_fm($time){
		//return date("d",$time)."<sup>".date("S", $time)."</sup> ".date("M", $time).date(" Y", $time).', '.date('h:i:s A', $time);
		if(empty($time)) return "";
		return "".date("M", $time).' '.date("d",$time).date("S", $time).' '.date(" Y", $time).', '.date('h:i:s A', $time);
	}
	
	function date_tr($time){
		//return date("d",$time)."<sup>".date("S", $time)."</sup> ".date("M", $time).date(" Y", $time).', '.date('h:i:s A', $time);
		
		return "".date("M", $time).' '.date("d",$time).date("S", $time).' '.date(" Y", $time);
	}
	
	function date_s($time){
		//return date("d",$time)."<sup>".date("S", $time)."</sup> ".date("M", $time).date(" Y", $time).', '.date('h:i:s A', $time);
		
		return "".date("M", $time).' '.date("d",$time).date("S", $time).' '.date(" Y", $time);
	}
	
	function check_status($bool, $true="Hidden", $false="Showing"){
		if($bool==0){
			return $true;
		}else{
			return $false;
		}
	}
	
	function display_small($content, $length="60", $start="0"){
		if(strlen($content) > $length){
			return substr($content,$start, $length)."...";
		}
		return $content;
	}
	
	function user_status($id){
		
		if($id==0){
			return "Saved";
		}elseif($id == 1){
			return "Sent for Approving";
		}elseif($id == 10){
			return "Approved";
		}
		
	}
	
    function sendmail1($to,$subject,$message,$name, $telephone="+256788229210"){ 
		return 0;
	}

    function sendmail($to,$subject,$message,$name, $telephone="+256788229210"){
		if(!isInternetOn()){
			FeedBack::warning("Email not sent, No Internet, Try checking the Network cables, Modem or Router");
			return 0;
		}
		require_once('PHPMailer-master/src/PHPMailer.php');
		require_once('PHPMailer-master/src/SMTP.php');
		$mail             = new PHPMailer();
		$body             = $message;
		$mail->IsSMTP();
		$mail->Host       = "smtp.gmail.com";                  
		$mail->SMTPAuth   = true;
		$mail->Host       = "smtp.gmail.com";
		$mail->Port       = 587;
		$mail->Username   = "josemusiitwa@gmail.com";
		$mail->Password   = "07067711712121";
		$mail->SMTPSecure = 'tls';
		$mail->SetFrom('josemusiitwa@gmail.com', 'UEDCL');
		$mail->AddReplyTo("josemusiitwa@gmail.com","UEDCL");
		$mail->Subject    = $subject;
		$mail->AltBody    = "To view the message, please use an HTML compatible email viewer!";
		$mail->MsgHTML($body);
		$address = $to;
		$mail->AddAddress($address, $name);
		if(!$mail->Send()) {
		  return 0;
		} else {
			return 1;
		}
    }
    function sms1($message, $number="+256788229210"){ return 0; }
	function sms($message, $number="+256788229210"){
		//return true;
		// Be sure to include the file you've just downloaded
		require_once('SMS/AfricasTalkingGateway.php');
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
	
	function n($x){}
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
	
	function password_generator(){
		//////////////////////////////////////////////////////////////////////////
		//		CODE generated by code eagles - joseph musiitwa - 2015/02/21	//
		//////////////////////////////////////////////////////////////////////////

		//range() increments two parameters and creates an array, they can be a third parameter for numbers ie step to increment.
		$low_case = range('a', 'z'); 
		$cap_letters = range('A', 'Z');
		$numbers = range(0, 9);
		$non_alpha_num = array('?', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '{', '}', '[', ']'); // you can add more as you want
		
		//reordering the array
		shuffle($low_case);
		shuffle($cap_letters);
		shuffle($numbers);
		shuffle($non_alpha_num);
		
		//the array collector is used to collect charaters
		//6 low case characters, 2 upper case, 2 numbers and 2 non-alphanumeric
		$collector = array( 
						$low_case[0], $low_case[1], 
						$cap_letters[0], $cap_letters[1],
						$numbers[0], $numbers[1], 
						$non_alpha_num[0], $non_alpha_num[1], 
						$low_case[2], $low_case[3],
						$low_case[4], $low_case[5],
						);
		
		shuffle($collector);//reordering the array
		
		foreach ($collector as $value) {
			@$generated_password .= $value;
		}
		
		return $generated_password;
	}
	
	
}

function trim_all( $str , $what = NULL , $with = ' ' )
	{				
		if	( $what === NULL )
		{
			//  Character      Decimal      Use
			//  "\0"            0           Null Character
			//  "\t"            9           Tab
			//  "\n"           10           New line
			//  "\x0B"         11           Vertical Tab
			//  "\r"           13           New Line in Mac
			//  " "            32           Space
		   
			$what   = "\\x00-\\x20";    //all white-spaces and control chars
		}
	   
		return trim( preg_replace( "/[".$what."]+/" , $with , $str ) , $what );
	}
	
	function str_replace_last( $search , $replace , $str ) {
		if( ( $pos = strrpos( $str , $search ) ) !== false ) {
			$search_length  = strlen( $search );
			$str    = substr_replace( $str , $replace , $pos , $search_length );
		}
		return $str;
	}

	function isInternetOn(){
    	$connected = @fsockopen("www.example.com", 80); 
    	if($connected){
        	$is_conn = true; 
        	fclose($connected);
    	}else{
        	$is_conn = false;
    	}
    	return $is_conn;

	}
	


?>