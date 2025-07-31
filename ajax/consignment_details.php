<?php
include "../classes/init.inc";

$consignment_id = $_POST['consignmentId'];

if(empty($consignment_id)) exit();

$db = new Db();
$t = new BeforeAndAfter();

$select = $db->select("SELECT * FROM consignment WHERE con_id = '$consignment_id'");
extract($select[0][0]);

if(empty($db->num_rows())) exit();

$db2 = new Db();
$sel = $db2->select("SELECT * FROM invoice WHERE inv_consignment_number = '$con_reference' AND inv_despatch IS NULL");

if(empty($db2->num_rows())) exit();
else $db2->num_rows();

$select = $db->select("SELECT name as con_deliver_to FROM valid_names WHERE category = '6' AND valid_name_id = '$con_deliver_to'");
extract($select[0][0]);


$select = $db->select("SELECT name as con_shipper FROM valid_names WHERE category = '7' AND valid_name_id = '$con_shipper'");
extract($select[0][0]);


echo '<div class="" style="margin-top:30px;">';

echo '<div class="col-md-6">';
echo '<div style="border:1px solid #999; padding:10px;">';
echo 'Consignee:<br/>';
echo '<div style="font-weight:bold; margin-bottom:12px">'.$con_deliver_to.'</div>';
echo '<div style="font-weight:bold; margin-bottom:12px">'.$t->rgf('consignee', $con_consignee, 'consignee_id', 'consignee_name').'</div>';
echo 'Transporter:<br/>';
echo '<div style="font-weight:bold; margin-bottom:12px">'.$con_shipper.'</div>';
echo 'To:<br/>';
echo '<div style="font-weight:bold; margin-bottom:12px">'.$con_deliver_to.'</div>';
echo 'Vehicle Nos.:<br/>';
echo '<div style="font-weight:bold; margin-bottom:12px">'.$con_vehicle_numbers.'</div>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-6">';
echo '<div style="border:1px solid #999; padding:10px;">';

echo '<input type="hidden" value="'.$con_consignment_notes.'" id="ccnn"/>';

echo 'Despatch Note No: ';
echo '<input type="text" id="notoNumber" value="" autocomplete="off" class="form-control"/>';

echo '<button type="button" style="margin-top:10px; margin-bottom:10px;" class="btn btn-primary btn-block" id="asConsignment">Assign as Consignment No</button>';

echo 'Despatch Date: ';
echo '<input type="date" style="margin-bottom:10px;" id="date" value="'.date('Y-m-d').'" class="form-control"/>';

echo 'Expected Arrival: ';
echo '<input type="date" id="arrival" value="'.date('Y-m-d', time()+24*60*60*4).'" class="form-control"/>';

?>
<script type="text/javascript">
	$(document).ready(function(){
		$('#asConsignment').click(function(){
			$('#notoNumber').val($('#ccnn').val());
		});
	});
</script>

<?php
echo '</div>';
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
echo '<th style="">Status</th>';
echo '<th style="text-align:center;">Despatched<br/>(Yes/No)</th>';
echo '</tr>';

$no=1;
$select = $db->select("SELECT * FROM invoice WHERE inv_consignment_number = '$con_reference' AND inv_despatch IS NULL ORDER BY inv_number ASC");
$total_b = $total_k = 0;
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
	echo '<td id="despatch'.$no.'">For Despatch</td>';
	echo '<td style="text-align:center;"><input id="checkDespatch'.$no.'" onchange="return despatcher('.$no.');" type="checkbox" value=""/><label for="checkDespatch'.$no.'"></label>';


	echo '<input type="hidden" value="'.$inv_id.'" id="invID'.$no.'"/>';

	echo '</td>';

	echo '</tr>';
}

echo '<tr>';
echo '<th colspan="3">Total</th>';
echo '<th>'.number_format($total_k).'</th>';
echo '<th>'.number_format($total_b).'</th>';
echo '<th colspan="2"></th>';
echo '</tr>';
echo '</table>';
?>
<script type="text/javascript">
	function despatcher(d){
		if($('#checkDespatch'+d).is(':checked')){
			if($('#notoNumber').val()==""){
				$('#despatch'+d).html('For Despatch');
				$('#checkDespatch'+d).prop('checked', false);
				alert("Please Enter Despatch Note No.");
			}else if($('#date').val()==""){
				$('#despatch'+d).html('For Despatch');
				$('#checkDespatch'+d).prop('checked', false);
				alert("Please Enter Despatch Date");
			}else if($('#arrival').val()==""){
				$('#despatch'+d).html('For Despatch');
				$('#checkDespatch'+d).prop('checked', false);
				alert("Please Enter Arrival Date");
			}else{
				//$('#despatch'+d).html('In Transit');

				var invoice = $('#invID'+d).val();
				var date = $('#date').val();
				var arrival = $('#arrival').val();
				var notoNumber = $('#notoNumber').val();

				var form_data = new FormData(); 
				form_data.append('invoice', invoice); 
				form_data.append('date', date); 
				form_data.append('arrival', arrival); 
				form_data.append('notoNumber', notoNumber); 

				$.ajax({
		        	xhr: function() {
		                var xhr = new window.XMLHttpRequest();
		                return xhr;
		            },
		            url: '<?php echo return_url().'ajax/despatch.php'; ?>',
		            type: "POST",
		            data: form_data,
		            contentType: false,
		            cache: false,
		            processData:false,
		            success: function(data){
		                console.log(data);
		                //alert(data);
		                $('#despatch'+d).html('In Transit');	
		            }
		        });
			}
		}else{
			$('#despatch'+d).html('For Despatch');			
		}
	}
</script>
<?php
echo '</div>';

echo '</div>';