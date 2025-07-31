<?php
include "../classes/init.inc";

$consignment_id = $_POST['consignmentId'];

if(empty($consignment_id)) exit();

$db = new Db();
$t = new BeforeAndAfter();

$select = $db->select("SELECT * FROM consignment WHERE con_id = '$consignment_id'");
extract($select[0][0]);

$select = $db->select("SELECT name as con_deliver_to FROM valid_names WHERE category = '6' AND valid_name_id = '$con_deliver_to'");
extract($select[0][0]);


$select = $db->select("SELECT name as con_shipper FROM valid_names WHERE category = '7' AND valid_name_id = '$con_shipper'");
extract($select[0][0]);


echo '<div class="" style="margin-top:0px;">';

echo '<div class="col-md-12" style="position:relative;">';

echo '<div style="border:1px solid #999; padding:10px; height:90px;">';
echo '<div style="margin-bottom:5px">Consignee: <b>'.$t->rgf("valid_names", $con_consignee, "valid_name_id", "name").'</b></div>';

echo '<div style="margin-bottom:5px">Shipped by: <b>'.$con_shipper.'</b></div>';

// echo '<div style="margin-bottom:12px">From: <b>'.$con_deliver_to.'</div>';
// echo 'Vehicle Nos.:';
// echo '<div style="font-weight:bold; margin-bottom:12px">'.$con_vehicle_numbers.'</div>';


echo '<div style="position:absolute;right:30px; top:10px;">';
echo '<select id="receivedAt" name="" class="select3" style="float:right;width:200px;display:inline-block!important">';

echo '<option value="">Select</option>';

$select = $db->select("SELECT * FROM valid_names WHERE category = '6'");//CONSIGNEE
foreach($select[0] as $row){
	extract($row);
	if($valid_name_id == $from)
		echo '<option selected value="'.$valid_name_id.'">'.$name.'</option>';
	else
		echo '<option value="'.$valid_name_id.'">'.$name.'</option>';
}
												
echo '</select>';

echo '<div style="float:right;width:80px;">Received at:</div>'; 
echo '</div>';
echo '<div style="position:absolute;right:30px; top:60px;">Received on: <input type="date" id="receivedOn" value=""></div>';

echo '</div>';
echo '</div>';

?>

<script type="text/javascript">
	$('#date1').bootstrapMaterialDatePicker({ 
		weekStart : 0, 
		time: false, 
		//minDate : new Date() 
	});

</script>


<?php

// echo '<div class="col-md-6">';
// echo '<div style="border:1px solid #999; padding:10px;">';

// echo '<input type="hidden" value="'.$con_consignment_notes.'" id="ccnn"/>';

// echo 'Despatch Note No: ';
// echo '<input type="text" id="notoNumber" value="" autocomplete="off" class="form-control"/>';

// echo '<button type="button" style="margin-top:10px; margin-bottom:10px;" class="btn btn-primary btn-block" id="asConsignment">Assign as Consignment No</button>';

// echo 'Despatch Date: ';
// echo '<input type="date" style="margin-bottom:10px;" id="date" value="'.date('Y-m-d').'" class="form-control"/>';

// echo 'Expected Arrival: ';
// echo '<input type="date" id="arrival" value="'.date('Y-m-d', time()+24*60*60*4).'" class="form-control"/>';

?>
<script type="text/javascript">
	$('.select3').select2();
	$(document).ready(function(){
		$('#asConsignment').click(function(){
			$('#notoNumber').val($('#ccnn').val());
		});
	});
</script>

<?php
// echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<br/>';
echo '<table id="table">';
echo '<tr valign="bottom">';
echo '<th style="">No.</th>';
echo '<th style="">Invoice No.</th>';
echo '<th style="">Grade</th>';
echo '<th style="">Quantity<br/>(Kgs)</th>';
echo '<th style="">No. of <br/>Bags</th>';
echo '<th style="">From</th>';
echo '<th style="">Despatch<br/>Note No</th>';
echo '<th style="">Warrant No</th>';
echo '<th style="">Kg Lost</th>';
echo '<th style="">Bags Left</th>';
echo '<th style="">Status</th>';
echo '</tr>';

$no=1;


$sql = "SELECT * FROM invoice WHERE inv_despatch_number = '$con_consignment_notes' AND inv_despatch IS NOT NULL AND inv_received IS NULL";
$select = $db->select($sql);

$db->num_rows();
$db->error();
$total_k = $total_b = 0;
foreach($select[0] as $row){
	extract($row);
	echo '<tr>';
	echo '<td>'.($no++).'.</td>';
	echo '<td>'.$inv_number.'</td>';
	echo '<td>'.$t->rgf('tea_grades', $inv_grade, 'tg_id', 'tg_grade').'</td>';
	echo '<td>'.number_format($inv_kgs).'</td>';
	$total_k += $inv_kgs;
	echo '<td>'.number_format($inv_bag_number).'</td>';
	$total_b += $inv_bag_number;
	echo '<td>'.$t->rgf('valid_names', $inv_from, 'valid_name_id', 'name').'</td>';
	echo '<td>'.$inv_despatch_number.'</td>';
	echo '<td></td>';
	echo '<td><input style="max-width:80px;" type="number" value="0" id="kg_lost'.$no.'"></td>';
	echo '<td><input style="max-width:80px;" type="text" class="number" value="'.number_format($inv_bag_number).'" id="bags_left'.$no.'"></td>';
	echo '<td>'; 
	echo '<select class="select3" name="" id="status'.$no.'" onchange="return receiver('.$no.');">';
	echo '<option value="Intransit">Intransit</option>';
	echo '<option value="Point of Sale">Point of Sale</option>';
	echo '<option value="Storage (Temp)">Storage (Temp)</option>';
	echo '<option value="Lost">Lost</option>';
	echo '</select>';

	echo '<input type="hidden" value="'.$inv_id.'" id="invoice'.$no.'">';
	echo '<input type="hidden" value="'.$inv_kgs.'" id="kg'.$no.'">';
	echo '<input type="hidden" value="'.$inv_bag_number.'" id="bag'.$no.'">';

	echo '</td>';


	echo '</tr>';
}

echo '<tr>';
echo '<th colspan="3"></th>';
echo '<th>'.number_format($total_k).'</th>';
echo '<th>'.number_format($total_b).'</th>';
echo '<th colspan="6"></th>';
echo '</tr>';

echo '</table>';
?>
<script type="text/javascript">
	
	function receiver(d){
		if($('#receivedAt').val()==""){
			$('#status'+d).val('Intransit');
			alert("Please Enter Received at");
		}else if($('#receivedOn').val()==""){
			$('#status'+d).val('Intransit');
			alert("Please Enter Received on");
		}else if($('#kg'+d).val()==""){
			$('#status'+d).val('Intransit');
			alert("Please Enter Kgs Lost");
		}else if($('#bag'+d).val()==""){
			$('#status'+d).val('Intransit');
			alert("Please Enter bags received");
		}else{
			
			var form_data = new FormData(); 
			form_data.append('received_on', $('#receivedOn').val()); 
			form_data.append('received_at', $('#receivedAt').val()); 
			form_data.append('kg', $('#kg'+d).val()); 
			form_data.append('bag', $('#bag'+d).val()); 
			form_data.append('invoice', $('#invoice'+d).val());
			form_data.append('status', $("#status"+d).val()); 

			$.ajax({
	        	xhr: function() {
	                var xhr = new window.XMLHttpRequest();
	                return xhr;
	            },
	            url: '<?php echo return_url().'ajax/received.php'; ?>',
	            type: "POST",
	            data: form_data,
	            contentType: false,
	            cache: false,
	            processData:false,
	            success: function(data){
	                console.log(data);	                
					$("#consignmentNumber option[value='"+$('#consignmentNumber').val()+"']").remove();
					//$("#consignmentDetails").html('<?php echo FeedBack::success("At Point of Sale"); ?>');
	            }
	        });
		}
		
	}
</script>
<?php
echo '</div>';

echo '</div>';

echo $db->error();