<?php 


$invNumber = $_POST['invNumber'];

error_reporting(null);
include "../classes/init.inc";
$t = new BeforeAndAfter();

$db = new Db();

$sql = "SELECT * FROM invoice WHERE inv_from = '$factory'";

$select = $db->select("SELECT * FROM invoice WHERE inv_id = '$invNumber' AND (inv_consignment_number IS NOT NULL or inv_consignment_number != 0) AND inv_type IS NULL");
//echo $db->num_rows();
echo '<br/>';




$i = 1;
foreach($select[0] as $row){
	extract($row);
echo '<input type="hidden" value="'.$inv_id.'" id="invoiceID"/>';
	echo '<div style="border:1px solid #ccc;">';

	echo '<div class="col-md-3">';
	echo 'Invoice Date: <b>'.FeedBack::date_s($inv_date_added).'</b>';
	echo '</div>';

	echo '<div class="col-md-2">';
	echo 'No: <b>'.$inv_number.'</b>';
	echo '</div>';

	echo '<div class="col-md-2">';
	echo 'Grade: <b>'.$t->rgf('tea_grades', $inv_grade, 'tg_id', 'tg_grade').'</b>';
	echo '</div>';

	echo '<div class="col-md-3">';
	echo 'From: <b>'.$t->rgf('valid_names', $inv_from, 'valid_name_id', 'name').'</b>';
	echo '</div>';

	echo '<div class="col-md-2">';
	echo 'No. Bags: <b>'.number_format($inv_bag_number).'</b>';
	echo '</div>';
	
	$zz = new Db();
	$a = "SELECT sum(u_reduce_kgs) as reduceKgs FROM unsold WHERE u_invoice_id = '$inv_id'";
	$zelect = $zz->select($a);
	extract($zelect[0][0]);
	$oinv_kgs = $inv_kgs;
	$inv_kgs -= $reduceKgs;
	
	echo '<div class="col-md-2">';
	echo 'Kgs: <b>'.number_format($oinv_kgs).'</b>';
	echo '<input type="hidden" value="'.($inv_kgs).'" id="soldKgs"/>';
	echo '</div>';

	echo '<div class="clearfix"></div>';

	echo '</div>';
	echo '<br/>';
	
	
	echo '<div style="border:1px solid #ccc;padding:10px; background-color:#fdecec;">';
	echo '<input type="checkbox" value="unsold" id="unsold"/><label for="unsold" style="line-height: 20px;">Unsold</label>';
	
	echo '&nbsp; &nbsp; &nbsp; Enter Kgs to reduce: <input id="reduceKgs" class="number" type="text" value="" min=0/>';	
	echo '&nbsp; &nbsp; &nbsp; Remaining Kgs: <span style="font-weight:bold;" id="remainingKgs">'.$inv_kgs.'</span>';	
	echo '<input type="hidden" value="'.$inv_kgs.'" id="oKgs"/>';
	echo '<br/>';
	echo '<div style="margin-top:10px;margin-bottom:0;">Reason/Comment:</div>';
	echo '<textarea style="width:100%;height:50px;" id="reason"></textarea>';
	echo '</div>';
	echo '<br/>';	


echo '<div class="row">';
echo '<div class="col-md-4">';
echo '<div style="border:1px solid #ccc;padding-top:20px;">';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Lot No.: &nbsp;</label>';
echo '<input type="text" id="lotNo" value=""/>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Broker: &nbsp;&nbsp;</label>';
$db = new Db();
$select = $db->select("SELECT * FROM valid_names WHERE category = '1005'");
//echo $db->num_rows();
echo '<select id="broker" class="select4" style="width:100%;">';
echo '<option value="">Select</option>';
foreach($select[0] as $row){
	extract($row);
	echo '<option value="'.$valid_name_id.'">'.$name.'</option>';
}
echo '</select>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Buyer: &nbsp;&nbsp;</label>';
$db = new Db();
$select = $db->select("SELECT * FROM valid_names WHERE category = '1008'");
//echo $db->num_rows();
echo '<select id="buyer" class="select4" style="width:100%;">';
echo '<option value="">Select</option>';
foreach($select[0] as $row){
	extract($row);
	echo '<option value="'.$valid_name_id.'">'.$name.'</option>';
}
echo '</select>';
echo '</div>';
echo '</div>';


echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Kgs Sold: &nbsp;&nbsp;</label>';
echo '<label style="text-align:right; font-weight:normal" id="soldLabel">'.number_format($inv_kgs).'</label>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>US$Gross: &nbsp;&nbsp;</label>';
echo '<input type="text" style="width:120px;" id="gross" value=""/>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>US$Costs: &nbsp;&nbsp;</label>';
echo '<label style="text-align:right; font-weight:normal" id="us_per_cost"></label>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>US$Net: &nbsp;&nbsp;</label>';
echo '<label style="text-align:right; font-weight:normal" id="net"></label>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Payment Date: &nbsp;&nbsp;</label>';
echo '<label style="text-align:right; font-weight:normal">'.FeedBack::date_s($inv_date_added+30*24*60*60).'</label>';
echo '</div>';
echo '</div>';

echo '<div class="clearfix"></div>';

echo '</div>';
echo '<br/>';
echo '<label>Notes: &nbsp;&nbsp;</label>';
echo '<textarea class="form-control" id="notes">'.$inv_notes.'</textarea>';
echo '</div>';


echo '<div class="col-md-8">';
echo '<div style="border:1px solid #ccc;padding:10px;">';
echo 'Costs:';

echo '<table id="table" style="width:100%" cellpadding="5">';

echo '<tr>';
echo '<th style="width:25%;"></th>';
echo '<th style="width:25%;">Rate</th>';
echo '<th style="width:25%;">Per</th>';
echo '<th style="width:25%;"></th>';
echo '</tr>';

echo '<tr>';
echo '<td>Transport</td>';
echo '<td><input style="width:100px; text-align:right;" id="trans" type="text" value="0"/></td>';
echo '<td>US$/Kg</td>';
echo '<td><input style="width:100px; text-align:right;" id="transT" type="text" value="'.number_format($inv_kgs*0, 2).'"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Brokerage</td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.75"/></td>';
echo '<td>%age</td>';
echo '<td><input style="width:100px; text-align:right;" id="brokerageT" type="text" value="0.00"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Warehousing</td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '<td>US$/Kg</td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Catalogue</td>';
echo '<td></td>';
echo '<td></td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Pallets</td>';
echo '<td></td>';
echo '<td></td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Insurance</td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '<td>US$/Kg</td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '</tr>';


echo '<tr>';
echo '<td>Bank</td>';
echo '<td></td>';
echo '<td></td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Allocation</td>';
echo '<td></td>';
echo '<td></td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '</tr>';

echo '<tr>';
echo '<td>Others</td>';
echo '<td></td>';
echo '<td></td>';
echo '<td><input style="width:100px; text-align:right;" type="text" value="0.00"/></td>';
echo '</tr>';

echo '<tr>';
echo '<th colspan="3" style="width:25%;"></th>';
echo '<th><input style="width:100px; text-align:right;" id="totalT" type="text" value="'.number_format(0+$inv_kgs*0,2).'" disabled /></th>';
echo '</tr>';

echo '<input style="width:100px; text-align:right;" id="totalT2" type="hidden" value="'.($inv_kgs*0).'" />';

echo '</table>';

echo '</div>';
echo '</div>';

echo '</div>';
}
?>

<script type="text/javascript">
	$(document).ready(function(){
		$('#reduceKgs').keyup(function(){
			var number = parseInt($(this).val());
			var oKgs = parseInt($("#oKgs").val());
			$("#remainingKgs").html(oKgs-number);	
			$("#soldLabel").html(oKgs-number);	
			$("#soldKgs").val(oKgs-number);			
		});
		$('#unsold').click(function(){
			if($('#unsold:checked').length > 0)
				$('#gross').attr('disabled',true);
			else
				$('#gross').attr('disabled',false);
		});
		$('.select4').select2();
		$('#gross').keyup(function(){
			var gross = $('#gross').val();
			var totalT = parseFloat($('#totalT2').val());
			if(gross!=""){				
				gross = parseFloat(gross);
			}else{
				gross = 0;
			}

			var total = gross*0.75/100;
			totalT += total;

			$('#brokerageT').val(total.toFixed(2));
			$('#totalT').val(totalT.toFixed(2));
			$('#us_per_cost').html(totalT.toFixed(2));
			$('#net').html((gross-totalT).toFixed(2));
			
		});
	});
</script>


