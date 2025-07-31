<?php 


$invNumber = $_POST['invNumber'];

error_reporting(null);
include "../classes/init.inc";
$t = new BeforeAndAfter();

$db = new Db();

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

	echo '<div class="col-md-2">';
	echo 'Kgs: <b>'.number_format($inv_kgs).'</b>';
	echo '<input type="hidden" value="'.($inv_kgs).'" id="soldKgs"/>';
	echo '</div>';

	echo '<div class="clearfix"></div>';

	echo '</div>';
	echo '<br/>';	


echo '<div class="row">';
echo '<div class="col-md-4"></div>';
echo '<div class="col-md-4">';
echo '<div style="border:1px solid #ccc;padding-top:20px;">';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Kgs Sold: &nbsp;&nbsp;</label>';
echo '<label style="text-align:right; font-weight:normal">'.number_format($inv_kgs).'</label>';
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
echo '<label>US$Net: &nbsp;&nbsp;</label>';
echo '<label style="text-align:right; font-weight:normal" id="net"></label>';
echo '</div>';
echo '</div>';

echo '<div class="col-md-12">';
echo '<div style="margin-bottom:5px;">';
echo '<label>Payment Date: &nbsp;&nbsp;</label>';
$p = $inv_date_added+30*24*60*60;
echo '<label style="text-align:right; font-weight:normal">'.FeedBack::date_s($p).'</label>';
echo '<input type="hidden" value="'.$p.'" id="paymentDate"/>';
echo '</div>';
echo '</div>';

echo '<div class="clearfix"></div>';

echo '</div>';
echo '<br/>';
echo '<label>Notes: &nbsp;&nbsp;</label>';
echo '<textarea class="form-control" id="notes">'.$inv_notes.'</textarea>';
echo '</div>';


echo '<div class="col-md-8">';
echo '</div>';

echo '</div>';
}
?>

<script type="text/javascript">
	$(document).ready(function(){
		$('#gross').keyup(function(){
			var gross = $('#gross').val();
			$('#net').html(gross);			
		});
	});
</script>


