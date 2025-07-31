<?php 

$security_url = 'http://sun-app.uetcl.com:8088/sunsystems-connect/soap/SecurityProvider';

$security_body = '<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:web="http://systemsunion.com/connect/webservices">
                     <soapenv:Body>
                         <web:SecurityProviderAuthenticateRequest>
                             <web:name>sunwasptest</web:name>
                             <web:password></web:password>
                         </web:SecurityProviderAuthenticateRequest>
                     </soapenv:Body>
                 </soapenv:Envelope>';
$ch = curl_init();

curl_setopt($ch, CURLOPT_URL, $security_url);
curl_setopt($ch, CURLOPT_POST, 1);
curl_setopt($ch, CURLOPT_POSTFIELDS, $security_body);
curl_setopt($ch, CURLOPT_HTTPHEADER, array(
    'Content-Type: text/xml; charset=UTF-8',
    'Content-Length: ' . strlen($security_body),
    'SOAPAction: http://systemsunion.com/connect/webservices/Authenticate',
));

curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

$response1 = curl_exec($ch);
$response1 = str_replace('soapenv:Envelope', 'envelope', $response1);
$response1 = str_replace('soapenv:Body', 'body', $response1);

$token = simplexml_load_string($response1);

//var_dump($token);

$response1 = $token->body->SecurityProviderAuthenticateResponse->response;
// print_r($token);
// print_r($token->body);

// Check for cURL errors
if (curl_errno($ch)) {
    //echo 'Curl error: ' . curl_error($ch);
} else {
    //echo 'Sun Token has been Aquired<br><br>'; 
}

$component_url = 'http://sun-app.uetcl.com:8088/sunsystems-connect/soap/ComponentExecutor';

// $str = "<SSC><User><Name>ZZZ</Name></User><SunSystemsContext><BusinessUnit>PBU</BusinessUnit></SunSystemsContext><Payload><MovementOrder><MovementOrderDefinitionCode>PBU_ISS</MovementOrderDefinitionCode><MovementOrderReference></MovementOrderReference><SecondReference>$req_number</SecondReference><Status></Status><TransactionReferenceNumber></TransactionReferenceNumber>";

$comp_payload ='<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:web="http://systemsunion.com/connect/webservices/">
    <soapenv:Body>
        <web:ComponentExecutorExecuteRequest>
            <web:authentication>'.$response1.'</web:authentication>
            <web:component>Customer</web:component>
            <web:method>CreateOrAmend</web:method>
            <web:group/>
            <web:payload><![CDATA[
                <SSC><User><Name>sunwasptest</Name></User>
                <SunSystemsContext>
                    <BusinessUnit>TCL</BusinessUnit>
                </SunSystemsContext>
                <Payload>
                    <Customer>
                        <CustomerCode>64053</CustomerCode>
						<ShortHeading>TEST1</ShortHeading>
                        <LookupCode>KEVIN</LookupCode>
						<ActualAccount>11000</ActualAccount>
						<Name>TEST1</Name>
						<Status>Open</Status>
                      
                   ';

$comp_payload .= '<Accounts>
						<LookupCode>KEVIN</LookupCode>
						<Status>Open</Status>
                  </Accounts>';



$comp_payload .= " </Customer>
                </Payload>
                </SSC>
            ]]></web:payload>
        </web:ComponentExecutorExecuteRequest>
    </soapenv:Body>
</soapenv:Envelope>";

$component_ch = curl_init();
curl_setopt($component_ch, CURLOPT_URL, $component_url);
curl_setopt($component_ch, CURLOPT_POST, 1);
curl_setopt($component_ch, CURLOPT_POSTFIELDS, $comp_payload);
curl_setopt($component_ch, CURLOPT_HTTPHEADER,array(
    'Host:sun-app.uetcl.com:8088',
    'Content-Type: text/xml charset=UTF-8',
    'Content-Length: ' . strlen($comp_payload),
    'SOAPAction: http://systemsunion.com/connect/webservices/Execute',
));
curl_setopt($component_ch, CURLOPT_RETURNTRANSFER, true);

$component_response = curl_exec($component_ch);
 $component_response = str_replace("\r\n", "", $component_response);
 $component_response = str_replace(">1", ">", $component_response);

if (curl_errno($component_ch)) {

    $error_message = 'Curl error: ' . curl_error($component_ch);
    echo json_encode(array("status" => "error", "details" => $error_message));
} else {

    $http_code = curl_getinfo($component_ch, CURLINFO_HTTP_CODE);
    
    if ($http_code === 200) {
        // Success: Process the ComponentExecutor response
        //echo '<br/><br/><br/>';
//         $select = $db->select("SELECT req_issue_reference FROM requisition WHERE req_id = '$id'");
//         extract($select[0]);

//         $req_issue_reference = str_replace("\r\n", "", $req_issue_reference);
// echo '<pre>';
// echo ($req_issue_reference);
// echo '</pre>';

// echo '@@@@<br/><br/>';
//         $req_issue_reference = new SimpleXMLElement((string)$req_issue_reference);

//         var_dump($req_issue_reference);

//         //echo $component_response;

//         echo  $req_issue_reference->Payload->MovementOrder->MovementOrderReference;

        //echo '<br/><br/><br/>';
		echo json_encode(array("status" => "success", "details" => "Payload sent to SunSystems \n Sun reference:", "response" => $component_response));
		//echo "<script>alert('Requisition Sent To SunSystems Successfully'); location.reload();</script>";
    } else {
        // Error: Handle non-200 HTTP response
        echo json_encode(array("status" => "error", "details" => "SunSystems returned HTTP code $http_code", "response" => $component_response));
    }
}
curl_close($component_ch);
curl_close($ch);



    