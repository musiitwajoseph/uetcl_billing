<?php 


$com_ref = $_POST['com_ref'];

if($com_ref==0) exit();

error_reporting(null);
include "../classes/Db.php";
include "../classes/BeforeAndAfter.inc";
$t = new BeforeAndAfter();

$db = new Db();

$select = $db->select("SELECT * FROM commerical_invoice WHERE com_ref = '$com_ref'");
extract($select[0][0]);


?>

<div class="row">
	<div class="col-lg-2">
		<div style="border:1px solid #ccc; padding:10px;margin-bottom:10px;">
			Commercial Invoice No.
			<br/><b><?php echo $com_number; ?></b>
		</div>
	</div>
	<div class="col-lg-10">
		<div style="border:1px solid #ccc; padding:10px;margin-bottom:10px;">
			<div class="row">
				<div class="col-lg-3">
					Sale Type
					<br/><b><?php echo $t->type_of_sale($com_sale_type); ?></b>
				</div>
				<div class="col-lg-4">
					Buyer	
					<br/><b><?php echo $t->rgf("valid_names", $com_buyer, "valid_name_id", "name"); ?></b>
					
				</div>
				<div class="col-lg-3">
					Sale Date					
					<br/><b><?php echo date('Y-m-d',$com_sale_date); ?></b>
					<input type="hidden" value="<?php echo date('Y-m-d',$com_sale_date); ?>" id="saleDate"/>
					<input type="hidden" value="<?php echo $com_number ?>" id="commericalInvoiceNumber"/>
				</div>
				<div class="col-lg-2">
					VAT (%)
					<br/><b><?php echo $com_tax; ?></b>
				</div>
				<div class="col-lg-12" style="margin-top:10px;">
					Notes 
					<br/><b><?php echo $com_notes; ?></b>
				</div>
			</div>
		</div>
	</div>
</div>

<?php
$select = $db->select("SELECT * FROM invoice WHERE inv_ref = '$com_ref'");
//echo $db->num_rows();
echo '<table id="table" border="1">';
echo '<tr>';
echo '<th>Invoice Number</th>';
echo '<th>Grade</th>';
echo '<th>Kgs</th>';
echo '<th>Bags</th>';
echo '<th>Ugx/Kg</th>';
echo '<th>Net Price</th>';
echo '<th>Sales Tax</th>';
echo '<th>Total Ugx</th>';
echo '<th></th>';
echo '</tr>';
$i = 1;
$total_kgs = $total_bags = $total_price = $total_tax = $total_w_tax = 0;
foreach($select[0] as $row){
	extract($row);
	$total_kgs += $inv_kgs;
	$total_bags += $inv_bag_number;
	$total_price += $inv_price;
	$total_tax += ($inv_price*$inv_kgs*($com_tax/100));
	$total_w_tax += ($inv_price*$inv_kgs*(1+$com_tax/100));
	echo '<tr>';
	echo '<td>'.$inv_number.'</td>';
	echo '<td>'.$t->rgf('tea_grades', $inv_grade, 'tg_id', 'tg_grade').'</td>';
	echo '<td align="right">'.number_format($inv_kgs).'</td>';
	echo '<td align="right">'.number_format($inv_bag_number).'</td>';
	echo '<td align="right">'.number_format($inv_price).'</td>';
	echo '<td align="right">'.number_format($inv_price*$inv_kgs).'</td>';
	echo '<td align="right">'.number_format($inv_price*$inv_kgs*($com_tax/100)).'</td>';
	echo '<td align="right">'.number_format($inv_price*$inv_kgs*(1+$com_tax/100)).'</td>';
	echo '<td align="right"><span id="sold'.$i.'">Awaiting Payment</span></td>';


	echo '</tr>';
	$i++;
}

echo '<tr style="font-weight:bold">';
echo '<td colspan="2" align="right">Total:</td>';
echo '<td align="right">'.number_format($total_kgs).'</td>';
echo '<td align="right">'.number_format($total_bags).'</td>';
echo '<td align="right"></td>';
echo '<td align="right"></td>';
//echo '<td align="right">'.number_format($total_price).'</td>';
echo '<td align="right">'.number_format($total_tax).'</td>';
echo '<td align="right">'.number_format($total_w_tax).'</td>';
echo '<td align="right"></td>';
echo '</tr>';

echo '</table>';

if(!$db->num_rows()) echo '<center><br/><b>No Invoices</b></center>';

echo '<input type="hidden" value="'.($i-1).'" id="allFields"/>';


