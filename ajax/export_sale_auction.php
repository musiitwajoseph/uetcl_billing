<?php 


$factory = $_POST['factory'];

error_reporting(null);
include "../classes/init.inc";
$t = new BeforeAndAfter();

$db = new Db();

$select = $db->select("SELECT name FROM valid_names WHERE valid_name_id = '$factory' ");
extract($select[0][0]);
echo '<h5>'.$name.' </h5>';
if($db->num_rows()){
	$sql = "SELECT * FROM invoice WHERE inv_from = '$factory' AND (inv_consignment_number IS NOT NULL or inv_consignment_number != 0) AND inv_type IS NULL";
	$select = $db->select($sql);
	//echo $db->num_rows();
	if($db->num_rows()){
		echo '<select id="invNumber" class="select3">';
		echo '<option value="">Select</option>';
		foreach($select[0] as $row){
			extract($row);
			echo '<option value="'.$inv_id.'">'.$inv_number.'</option>';
			$i++;
		}
		echo '</select>';

		echo '<div id="invNumberDetails"></div>';

		?>
		<script type="text/javascript">
			$(document).ready(function(){				
				$('.select3').select2();
				$('#invNumber').change(function(){				
					var form_data = new FormData(); 
					form_data.append('invNumber', $('#invNumber').val()); 

					$('#invNumberDetails').html("Loading Data.");

					if($('#invNumber').val()!=""){
						$.ajax({
				        	xhr: function() {
				                var xhr = new window.XMLHttpRequest();
				                return xhr;
				            },
				            url: '<?php echo return_url().'ajax/export_invoice_details.php'; ?>',
				            type: "POST",
				            data: form_data,
				            contentType: false,
				            cache: false,
				            processData:false,
				            error:function(jqXHR, exception){
				            	var msg = '';
						        if (jqXHR.status === 0) { 
						            msg = 'Not connect. <br/> Verify Network.';
						        } else if (jqXHR.status == 404) {
						            msg = 'Requested page not found. [404]';
						        } else if (jqXHR.status == 500) {
						            msg = 'Internal Server Error [500].';
						        } else if (exception === 'parsererror') {
						            msg = 'Requested JSON parse failed.';
						        } else if (exception === 'timeout') {
						            msg = 'Time out error.';
						        } else if (exception === 'abort') {
						            msg = 'Ajax request aborted.';
						        } else {
						            msg = 'Uncaught Error.' + jqXHR.responseText;
						        }
				                $('#invNumberDetails').html('<p style="color:#EA4335;">'+msg+'<br/>Upload failed, please try again.</p>');
				            },
				            success: function(data){
				                console.log(data);
				                $('#invNumberDetails').html(data);
				            }
				        });
					}
			    });
			});	
		</script>
		<?php

		echo '<div id="invNumberDetails"></div>';
	}else{
		echo 'No Invoices';
	} 
}

//////////////////////////////////////////////////////

// $select = $db->select("SELECT * FROM invoice WHERE inv_from = '$factory' AND (inv_consignment_number IS NULL or inv_consignment_number = '')");
// //echo $db->num_rows();
// foreach($select[0] as $row){
// 	extract($row);
// 	echo '<td>'.$inv_number.'</td>';
// 	echo '<td>'.$t->rgf('tea_grades', $inv_grade, 'tg_id', 'tg_grade').'</td>';
// 	echo '<td>'.number_format($inv_kgs).'</td>';
// 	echo '<td>'.number_format($inv_bag_number).'</td>';
// 	echo '<td>'; 

// 	echo '<input type="text" autocomplete="off" class="number" name="" value="" id="aa'.$i.'" onkeyup="return allow(\''.$i.'\');"/>';
// 	echo '<input type="hidden" value="'.$inv_id.'" id="a'.$i.'"/>';
// 	echo '<input type="hidden" value="0" id="t'.$i.'"/>';
// 	echo '<input type="hidden" value="'.$inv_kgs.'" id="kgs'.$i.'"/>';
// 	echo '<input type="hidden" value="'.$inv_bag_number.'" id="bags'.$i.'"/>';
// 	echo '</td>';
// 	echo '<td align="right"><span id="netPrice'.$i.'">0</span></td>';
// 	echo '<td align="right"><span id="salesTax'.$i.'">0</span></td>';
// 	echo '<td align="right"><span id="totalWithTax'.$i.'">0</span></td>';
// 	echo '<td align="right"><span id="sold'.$i.'">Unsold</span></td>';


// 	echo '</tr>';
// 	$i++;
// }

// echo '</table>';

// if(!$db->num_rows()) echo '<center><br/><b>No Invoices</b></center>';

// echo '<input type="hidden" value="'.($i-1).'" id="allFields"/>';


