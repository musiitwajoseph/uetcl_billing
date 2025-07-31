<?php    
function qrcode($text, $time=11){   
    //$text = "musiitwa joseph";
    //set it to writable location, a place for temp generated PNG files
    $PNG_TEMP_DIR = dirname(__FILE__).DIRECTORY_SEPARATOR.'temp'.DIRECTORY_SEPARATOR;
    
    //html PNG location prefix
    $PNG_WEB_DIR = 'images/'; 
    
    //ofcourse we need rights to create temp dir
    if (!file_exists($PNG_TEMP_DIR))
        mkdir($PNG_TEMP_DIR);
 	
	$errorCorrectionLevel = 'H'; 
    $matrixPointSize = 4;
	$matrixPointSize = min(max(5, 1), 10);

    if (isset($text)) {     
        //it's very important!
        // user data
        $filename = 'images/EFRIS'.md5($text.'|'.$errorCorrectionLevel.'|'.$matrixPointSize).'.png';
       QRcode::png($text, $filename, $errorCorrectionLevel, $matrixPointSize, 2);    
        echo '<img src="http://localhost:8080/waternew/images/'.basename($filename).'?v='.$time.'" alt="'.$filename.'"/>'; 
    } else {  
        echo "Not uploaded...";
    }      
}  

    