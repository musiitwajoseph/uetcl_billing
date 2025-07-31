<?php
Class EfrisGoods extends BeforeAndAfter{
	public $page = "EFRIS GOODS";
	
	public function __construct(){
		$access = new AccessRights();
		//$access->pageAccess(user_id(), $this->page, 'V');
	}
	
	public function getLinks(){
		$page = "EFRIS GOODS";
		$es = new EfrisGoods();
		$p = $es->pending();
		$p = ($p)?"(".$p.")":"";
		$links = array(
			array(
				"link_name"=>"Add Good", 
				"link_address"=>"efris-goods/add-good",
				"link_icon"=>"fa-plus",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Pending".$p, 
				"link_address"=>"efris-goods/pending-goods",
				"link_icon"=>"fa-refresh",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Interfaced Goods", 
				"link_address"=>"efris-goods/interfaced-goods",
				"link_icon"=>"fa-check",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			// array(
			// 	"link_name"=>"Available Stock", 
			// 	"link_address"=>"efris-goods/available-stock",
			// 	"link_icon"=>"fa-shopping-basket",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// ),
			// array(
			// 	"link_name"=>"Signature T104", 
			// 	"link_address"=>"efris-goods/signature-t104",
			// 	"link_icon"=>"fa-pencil",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// ),
			array(
				"link_name"=>"Exchange Rate T126", 
				"link_address"=>"efris-goods/exchange-rate-t126",
				"link_icon"=>"fa-money",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Check Taxpayer T137", 
				"link_address"=>"efris-goods/check-taxpayer-t137",
				"link_icon"=>"fa-money",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Goods & Services COMMODITY CODE or CATEGORY ID", 
				"link_address"=>"goods/goods-and-services",
				"link_icon"=>"fa-table",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			array(
				"link_name"=>"Import Goods & Services", 
				"link_address"=>"goods/import-goods-and-services",
				"link_icon"=>"fa-upload",
				"link_page"=>$page,
				"link_right"=>"V",
			),
			// array(
			// 	"link_name"=>"check T115", 
			// 	"link_address"=>"efris-goods/dictionary-T115",
			// 	"link_icon"=>"fa-money",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// ),
			// array(
			// 	"link_name"=>"Available Stock", 
			// 	"link_address"=>"efris-stock/available-stock",
			// 	"link_icon"=>"fa-check",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// ),
			// array(
			// 	"link_name"=>"Other Invoices - Increase Stock", 
			// 	"link_address"=>"efris-stock/other-invoices-increase-stock",
			// 	"link_icon"=>"fa-arrow-up",
			// 	"link_page"=>$page,
			// 	"link_right"=>"V",
			// ),
		);
		
		return $links;
	}

	public function exchangeRateT126Action(){
		$db = new Db();
		$efris = new Efris();
		$i=1;
		
		//$this->start_print("Exchange Rates T126");

		echo '<div class="stock"></div>';
		
		echo '<div id="stockStatus" style="clear:both;margin-top:20px;"></div>';
		
		//$this->end_print();
		
		?>
		<script type="text/javascript">
		$(document).ready(function(){
			var urlPath = $('#urlPath').val();
			
			$('.stock').each(function(){
				var id = $(this).val();
				$(this).html('checking...');
				var form_data = new FormData();
				form_data.append('id', 'id');
				//alert(id);
				$.ajax({
					url: urlPath+"ajax/good_t126.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('.stock').html(data);
						var	values = JSON.parse(data);
						//$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#stockStatus').html(values.error);
						}else{
							$('#stockStatus').append('<div>'+values.response+'</div>');	
							$(".stock").html(values.returned);				
						}
					}		
				});
			});
		});
		</script>
		
		<?php
	}
	
	

	public function dictionaryT115Action(){
		$db = new Db();
		$efris = new Efris();
		$i=1;
		
		//$this->start_print("System Dictionary T115");

		echo '<div class="stock"></div>';
		
		echo '<div id="stockStatus" style="clear:both;margin-top:20px;"></div>';
		
		//$this->end_print();
		
		?>
		<script type="text/javascript">
		$(document).ready(function(){
			var urlPath = $('#urlPath').val();
			
			$('.stock').each(function(){
				var id = $(this).val();
				$(this).html('checking...');
				var form_data = new FormData();
				form_data.append('id', 'id');
				//alert(id);
				$.ajax({
					url: urlPath+"ajax/T115_dictionary.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('.stock').html(data);
						var	values = JSON.parse(data);
						//$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#stockStatus').html(values.error);
						}else{
							$('#stockStatus').append('<div>'+values.response+'</div>');	
							$(".stock").html(values.returned);				
						}
					}		
				});
			});
		});
		</script>
		
		<?php
	}
	
	public function signatureT104Action(){
		$db = new Db();
		$efris = new Efris();
		$i=1;
		
		//$this->start_print("Signature T104");

		echo '<div class="stock"></div>';
		
		echo '<div id="stockStatus" style="clear:both;margin-top:20px;"></div>';
		
		//$this->end_print();
		
		?>
		<script type="text/javascript">
		$(document).ready(function(){
			var urlPath = $('#urlPath').val();
			
			$('.stock').each(function(){
				var id = $(this).val();
				$(this).html('checking...');
				var form_data = new FormData();
				form_data.append('id', 'id');
				//alert(id);
				$.ajax({
					url: urlPath+"ajax/T104_signature.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('.stock').html(data);
						var	values = JSON.parse(data);
						//$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#stockStatus').html(values.error);
						}else{
							$('#stockStatus').append('<div>'+values.response+'</div>');	
							$(".stock").html(values.returned);				
						}
					}		
				});
			});
		});
		</script>
		
		<?php
	}
	

	public function checkTaxpayerT137Action(){
		$db = new Db();
		$efris = new Efris();
		$i=1;
		
		//$this->start_print("Exchange Rates T126");
		echo '<div style="border:1px solid #000; padding:20px;">';
		echo 'TIN: <input type="text" value="" id="tin"/>&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp;';
		echo 'Commodity Category Code(s): <input id="ccc" type="text" value="" placeholder="10000000,10000001" style="width:400px;"/>&nbsp;&nbsp;<br/>';
		echo '<button id="check" style="margin-top:20px;"><i class="fa fa-fw fa-refresh"></i> Check</button>';
		echo '<span id="checkBtn"></span>';
		echo '</div>';
		
		echo '<div id="stockStatus" style="clear:both;margin-top:20px;"></div>';
		
		//$this->end_print();
		
		?>
		<script type="text/javascript">
		$(document).ready(function(){
			var urlPath = $('#urlPath').val();
			
			$('#check').click(function(){
				$("#checkBtn").html('checking...');
				var form_data = new FormData();
				form_data.append('tin', $('#tin').val());
				form_data.append('ccc', $('#ccc').val());
				//alert(id);
				$.ajax({
					url: urlPath+"ajax/checkTaxpayer.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#stockStatus').html(data);

						$("#checkBtn").html('');
						//var	values = JSON.parse(data);
						//$('#pushInvoiceToEFRIS').attr("disabled",false);
						// if(values.status == "error"){
						// 	//$('#stockStatus').html(values.error);
						// }else{
						// 	//$('#stockStatus').append('<div>'+values.response+'</div>');	
						// 	//$(".stock").html(values.returned);				
						// }
					}		
				});
			});
		});
		</script>
		
		<?php
	}
	

	public function addGoodAction(){
		$id = portion(3);
		$db = new Db();

?>
		<div class="" style="margin-top:10px;">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">

						<form role="form" action="" method="post">
							<div class="col-md-12">
								<div id="must">All field with asterisk(*) are mandatory.</div>
							</div>
							<div class="clearfix"></div>

							<div class="col-lg-2">
								<div class="form-group">
									<label>Item Code<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodCode" value="<?php echo @$eg_code; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-4">
								<div class="form-group">
									<label>Item Name<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodName" value="<?php echo @$eg_name; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>OperationType<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="OperationType">
											<?php
											$ot = array('101'=>'Add Good', '102'=>'Modify Good');
											//$ot = array('101'=>'Add Good');
											foreach($ot as $o=>$t){
												echo '<option data-subtext="" value="'.$o.'">'.$t.'</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>	
							<!-- <div class="col-lg-2">
								<div class="form-group">
									<label>Good Code<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodCode" value="<?php echo @$eg_code; ?>">
									</div>
								</div>
							</div>
							<div class="clearfix"></div>
							<div class="col-lg-3">
								<div class="form-group">
									<label>Good Name<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodName" value="<?php echo @$eg_name; ?>">
									</div>
								</div>
							</div> -->
							<div class="col-lg-2">
								<div class="form-group">
									<label>Unit Price<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="unitPrice" value="100">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>UOM<span class="must">*</span></label>
									<div class="form-line">
										<select id="uom" style="width:100%;">
											<option value="">Select</option>
											<?php 
											$db = new Db();
											$sql = "SELECT * FROM dictionary WHERE d_category = '2' ORDER BY d_name ASC";
											$select = $db->select($sql);
											foreach($select[0] as $row){
												extract($row);
												echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
											}
											?>
										</select>
									</div>
									
								</div>
							</div>
							<div class="clearfix"></div>
							<div class="col-lg-3">
								<div class="form-group">
									<label>Category Id<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" id="categoryID" value="">
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Has excise TAX?<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="hasExciseTax">
											<?php
											$ot = array('101'=>'Yes', '102'=>'No');
											foreach($ot as $o=>$t){
												echo '<option data-subtext="" value="'.$o.'">'.$t.'</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Currency<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="currency">
											<?php
											$db = new Db();
											$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
											$select = $db->select($sql);
											foreach($select[0] as $row){
												extract($row);
												if($eg_currency == $d_code)
												echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
												echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Stock Warning<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" id="stockWarning" value="10">
									</div>
								</div>
							</div>						
							<!-- <input type="hidden" value="<?php echo $eg_id; ?>" id="goodID"/> -->
							<div class="clearfix"></div>
							<div class="col-lg-12">
								<button id="efrisGood" type="button" onclick="return confirm('Are you sure you want to save.');"name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								<span id="efrisGoodStatus"></span>
							</div>
							<script type="text/javascript">		
								var urlPath = $('#urlPath').val();
								$('#efrisGood').click(function(){
									$('#efrisGoodStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
									$(this).attr("disabled", true);
									var items = $('#items').val();
									if(1){
										//alert( $('#OperationType').val());
										var form_data = new FormData();				
										var form_data = new FormData();				

										form_data.append('goodCode', $('#goodCode').val());				
										form_data.append('goodName', $('#goodName').val());
										form_data.append('OperationType', $('#OperationType').val());		
										form_data.append('categoryID', $('#categoryID').val());	
										// // form_data.append('goodID', $('#goodID').val());				
										// form_data.append('goodName', $('#goodName').val());
										form_data.append('unitPrice', $('#unitPrice').val());
										form_data.append('uom', $('#uom').val());
										form_data.append('hasExciseTax', $('#hasExciseTax').val());
										form_data.append('categoryID', $('#categoryID').val());
										form_data.append('currency', $('#currency').val());
										form_data.append('stockWarning', $('#stockWarning').val());
										
										
										$.ajax({
											url: urlPath+"ajax/_addGood.php",
											type: "POST",
											data: form_data,
											contentType: false,
											cache: false,
											processData:false,
											success: function(data){			
												//alert(data);	
													//$('#efrisGoodStatus').append(data+"<br/><br/>");	
												var	values = JSON.parse(data);
												$('#efrisGood').attr("disabled",false);
												if(values.status == "error"){
													$('#efrisGoodStatus').html(values.error);
												}else{
													$('#efrisGoodStatus').html('Saved');
													location.reload();							
													$('#efrisGoodStatus').append(values.response);							
												}
											}
										});
									}else{
										//alert("Enter Lot No.");
										$('#efrisGoodStatus').html("");
									}
								});
							</script>
						</form>
					</div>
				</div>
			</div>
		</div>
		<?php	
	}
	
	public function availableStockAction(){
		$db = new Db();
		$efris = new Efris();
		$i=1;
		
		//$this->start_print("AVAILABLE STOCK");
		echo '<table id="table" border="1" cellpadding="2" cellspacing="0" style="font-size:12px; width:100%;">';
		echo '<tr>';
		echo '<th style="width:30px;">No.</th>';
		echo '<th>Goods</th>';
		echo '<th>Goods Code</th>';
		echo '<th>ID</th>';
		echo '<th style="text-align:right;">Stock Prewarning</th>';
		echo '<th style="text-align:right;">Quantity</th>';
		echo '</tr>';
		foreach($efris->stockGoods() as $type => $values){
			if($values[7]==1){
				echo '<tr>';
				echo '<td>'.($i++).'.</td>';
				echo '<td>'.strtoupper($type).'</td>';
				echo '<td>'.$values[1].'</td>';
				echo '<td><input type="hidden" class="stock" value="'.$values[0].'"/> '.$values[0].'</td>';
				echo '<td style="text-align:right;" id="td_pre'.$values[0].'"></td>';
				echo '<td style="text-align:right;" id="td'.$values[0].'"></td>';
				echo '</tr>';
			}
		}
		echo '</table>';
		echo '<div style="padding:10px">Last checked: '.FeedBack::date_fm(time()).'</div>';
		//$this->end_print();
		
		echo '<a href="" style="margin-top:10px" class="pull-left btn btn-danger btn-xs"><i class="fa fa-fw fa-refresh"></i> Reload</a>';
		
		//echo '<button style="margin-top:10px" type="button" class="pull-right btn btn-danger btn-xs"><i class="fa fa-fw fa-arrow-down"></i> Decrease Stock</button>';
		
		echo '<div>';
		
		echo '<div style="clear:both;"><Br/></div>';
		echo '<div style="border:1px solid black; padding:10px;">';
		echo '<h5 style="color:red;">Decrease Stock form </h5>';
		echo '<div style="float:left;padding:10px;">';
		echo 'Select Good:<br/><select id="good" style="width:150px;">';		
		foreach($efris->stockGoods() as $type => $values){
			echo '<option value="'.$type.'">'.strtoupper($type) .' ('.$values[1].')</option>';
		}
		echo '</select>';
		echo '</div>';
		
		echo '<div style="float:left; padding:10px;">';
		echo 'Quantity:<br/><input type="number" min="1" value="" id="quantity"/>';
		echo '</div>';
		$t = array(
			'101'=>'Expired Goods',
			'102'=>'Damaged Goods',
			'103'=>'Personal Uses',
		);
				
		echo '<div style="float:left;padding:10px;">';
		echo 'Select Adjust Type:<br/><select id="adjustType" style="width:150px;">';		
		foreach($t as $type => $value){
			echo '<option value="'.$type.'">'.$value.'</option>';
		}
		echo '</select>';
		echo '</div>';
		
		echo '<div style="float:left; padding:10px;">';
		echo 'Reason/Remarks:<br/><textarea id="comment"></textarea>';
		echo '</div>';
		
		echo '<div style="float:left; padding:10px;">';
		echo '&nbsp;<br/><button type="button" class="btn btn-xs btn-danger" id="decrease"><i class="fa fa-fw fa-arrow-down"></i>Decrease</button>';
		echo '</div>';
		
		echo '<div style="clear:both;"></div>';
		
		echo '</div>';
		
		echo '</div>';
		
		echo '<div id="stockStatus" style="clear:both;margin-top:20px;"></div>';
		
		
		?>
		<script type="text/javascript">
		$(document).ready(function(){
			var urlPath = $('#urlPath').val();
			
			$('#decrease').click(function(){
			
				var good = $('#good').val();
				var quantity = $('#quantity').val();
				var adjustType = $('#adjustType').val();
				var comment = $('#comment').val();
				
				var form_data = new FormData();
				form_data.append('good', good);
				form_data.append('quantity', quantity);
				form_data.append('adjustType', adjustType);
				form_data.append('comment', comment);
				
				//alert(id);
				$.ajax({
					url: urlPath+"ajax/decrease_stock.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						//alert(data);
						var	values = JSON.parse(data);
						//$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#stockStatus').html(values.error);
						}else{
							$('#stockStatus').html('');
							$('#stockStatus').append("<span style='color:green;'>"+values.total+" &nbsp; &nbsp; "+values.reference+'</span>');
							$('#stockStatus').append(values.response);
						}
					}
				});			
			});
			
			$('.stock').each(function(){
				var id = $(this).val();
				$('#td'+id).html('checking...');
				var form_data = new FormData();
				form_data.append('id', id);
				//alert(id);
				$.ajax({
					url: urlPath+"ajax/stock_quantity.php",
					type: "POST",
					data: form_data,
					contentType: false,
					cache: false,
					processData:false,
					success: function(data){
						$('#stockStatus').html(data);
						var	values = JSON.parse(data);
						//$('#pushInvoiceToEFRIS').attr("disabled",false);
						if(values.status == "error"){
							$('#stockStatus').html(values.error);
						}else{
							$('#stockStatus').append('<div>'+values.response+'</div>');							
							$('#td'+id).html(values.returned_stock.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ","));								
							$('#td_pre'+id).html(values.returned_warning.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ","));											
						}
					}		
				});
			});
		});
		</script>
		
		<?php
	}
	public function pendingGoodsAction(){

		$xx = array(
	    "goodsCode"=>"Eskom002",
	);
/*
		$efris = new Efris();

	$xv = json_encode($xx);
	$xvv = base64_encode($xv);

	$xdata_to_send = array(
	    "interface"=>"T144",
	    "request"=>$xvv,
	    "request_time"=>date('Y-m-d H:i:s'),
	);

	//print_r($xdata_to_send);
	//$xdata = str_replace('\/', '/', $xefris->data_to_send($xdata_to_send));
	$xdata = str_replace('"@@#####@@"', '{}', $efris->data_to_send($xdata_to_send));

	$xresponse = $efris->send_and_receive($xdata, 'http://127.0.0.1:9880/efristcs/ws/tcsapp/getInformation');

	$xr = json_decode($xresponse);

	$xcontent = $xr->data->content;
	$xc = base64_decode($xcontent);
	$xc = json_decode($xc);
	var_dump($xc);
	echo $xc[0]->id;
	*/

		$db = new Db();
		$sql = "SELECT * FROM efris_goods WHERE eg_mapped IS NULL";
		$select = $db->select($sql);

		//$access = new AccessRights();
	?>
		<div class="col-md-12">
			<?php 
			
			if(!$select){
				echo $db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr valign="bottom">';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				echo '<th>Code</th>';
				echo '<th>Good Name</th>';
				// if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V')){
				echo '<th width="">Action</th>';
				// }
				echo '</tr>';
				echo '</thead>';
////////////////////////////////////report step1/////////////////////////////////////
			
////////////////////////////////////////////////////////////////////////////////				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					$color = (!$eg_mapped)?'style="background-color:#f5d3d3"':""; 
					echo "<tr $color>";
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.Feedback::date_fm($eg_date_added).'</td>';
					echo '<td>'.($eg_code).'</td>';
					echo '<td>'.($eg_name).'</td>';
					//echo '<td>'.($check_number).'</td>';
					

					//if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V'))
					{
						echo '<td>';

						echo '<a class="btn btn-xs btn-primary" href="'.return_url().'/efris-goods/create-good/'.$eg_id.'"><i class="fa fa-fw fa-check"></i> Create Good</a>';
						echo ' &nbsp; ';
						echo '<a class="btn btn-xs btn-danger" onclick="return confirm(\'Do you want to Delete this Good?\');" href="'.return_url().'/efris-goods/delete-good/'.$eg_id.'"><i class="fa fa-fw fa-trash"></i> Delete</a>';
					
						echo '</td>';
					}
					echo '</tr>';
					
					 
				}
				echo '</tbody>';
				
				echo '</table>';

			}
			
			?>
		</div>
		<?php
	
	}

	public function deleteGoodAction(){
		$id = portion(3);		
		$this->deletor("efris_goods", "eg_id",  $id, 'efris-goods/pending-goods');
	}



	public function interfacedGoodsAction(){

		$db = new Db();
		$sql = "SELECT * FROM efris_goods WHERE eg_mapped IS NOT NULL";
		$select = $db->select($sql);

		//$access = new AccessRights();
	?>
		<div class="col-md-12">
			<?php 
			
			if(!$select){
				echo $db->error();
			}else{
				echo '<table cellspacing="0" cellpadding="2" border="1" width="100%" id="table">';
				echo '<thead>';
				echo '<tr valign="bottom">';
				echo '<th width="30px">No.</th>';
				echo '<th>Date</th>';
				echo '<th>Category Id</th>';
				echo '<th>Code</th>';
				echo '<th>Good Name</th>';
				echo '<th>UOM</th>';
				echo '<th>Goods Id</th>';
				// if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V')){
				echo '<th width="">Action</th>';
				// }
				echo '</tr>';
				echo '</thead>';
////////////////////////////////////report step1/////////////////////////////////////
			
////////////////////////////////////////////////////////////////////////////////				
				$i=1;
				echo '<tbody>';
				foreach($select[0] as $row){
					extract($row);
					$color = (!$eg_mapped)?'style="background-color:#f5d3d3"':""; 
					echo "<tr $color>";
					echo '<td><center>'.($i++).'.</center></td>';
					echo '<td>'.Feedback::date_fm($eg_date_added).'</td>';
					echo '<td>'.($eg_category_id).'</td>';
					echo '<td>'.($eg_code).'</td>';
					echo '<td>'.($eg_name).'</td>';
					echo '<td>'.$this->rgf("dictionary",$eg_uom,"d_code","d_name").'</td>';
					echo '<td>'.($eg_good_id).'</td>';
					//echo '<td>'.($check_number).'</td>';
					

					//if($access->sectionAccess(user_id(), $this->page, 'E') || $access->sectionAccess(user_id(), $this->page, 'V'))
					{
						echo '<td>';

						echo '<a class="btn btn-xs btn-danger" href="'.return_url().'/efris-goods/modify-good/'.$eg_id.'">Modify Good</a>';
					
						echo '</td>';
					}
					echo '</tr>';
					
					 
				}
				echo '</tbody>';
				
				echo '</table>';

			}
			
			?>
		</div>
		<?php
	
	}

	public function modifyGoodAction(){
		$id = portion(3);
		$db = new Db();
		$sql = "SELECT * FROM efris_goods WHERE eg_id = '$id'";
		$select = $db->select($sql);
		extract($select[0][0]);

		echo '<b>Item code: </b>'.$eg_code;
		echo '<br/>';
		echo '<b>Name: </b>'.$eg_name;
?>
		<div class="" style="margin-top:10px;">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">

						<form role="form" action="" method="post">
							<div class="col-md-12">
								<div id="must">All field with asterisk(*) are mandatory.</div>
							</div>
							<div class="clearfix"></div>

							<div class="col-lg-2">
								<div class="form-group">
									<label>OperationType<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="OperationType">
											<?php
											$ot = array('101'=>'Add Good', '102'=>'Modify Good');
											//$ot = array('102'=>'Modify Good');
											foreach($ot as $o=>$t){
												if($o ==$eg_operation_type)
												echo '<option selected data-subtext="" value="'.$o.'">'.$t.'</option>';
												else
													echo '<option data-subtext="" value="'.$o.'">'.$t.'</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Good Code<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodCode" value="<?php echo @$eg_code; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-4">
								<div class="form-group">
									<label>Good Name<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodName" value="<?php echo @$eg_name; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Unit Price<span class="must"></span></label>
									<div class="form-line">
										<?php if($eg_unit_price == 0) $eg_unit_price = ""; ?>
										<input class="form-control" type="text" autocomplete="off" id="unitPrice" value="<?php echo $eg_unit_price;?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>UOM<span class="must">*</span></label>
									<div class="form-line">
										<select id="uom" style="width:100%;">
											<option value="">Select</option>
											<?php 
											$db = new Db();
											$sql = "SELECT * FROM dictionary WHERE d_category = '2' ORDER BY d_name ASC";
											$select = $db->select($sql);
											foreach($select[0] as $row){
												extract($row);
												if($eg_uom == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>
							<div class="clearfix"></div>
							<div class="col-lg-3">
								<div class="form-group">
									<label>Category Id<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" id="categoryID" value="<?php echo$eg_category_id;?>">
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Has excise TAX?<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="hasExciseTax">
											<?php
											$ot = array('101'=>'Yes', '102'=>'No');
											foreach($ot as $o=>$t){
												if($o ==$eg_has_excise_tax)
												echo '<option selected data-subtext="" value="'.$o.'">'.$t.'</option>';
												else
												echo '<option data-subtext="" value="'.$o.'">'.$t.'</option>';	
											}
											?>
										</select>
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Currency<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="currency">
											<?php
											$db = new Db();
											$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
											$select = $db->select($sql);
											foreach($select[0] as $row){
												extract($row);
												if($eg_currency == $d_code)
												echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
												echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Stock Warning<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" id="stockWarning" value="<?php echo$eg_stock_warning;?>">
									</div>
								</div>
							</div>						
							<input type="hidden" value="<?php echo $eg_id; ?>" id="goodID"/>
							<div class="clearfix"></div>
							<div class="col-lg-12">
								<button id="efrisGood" type="button" onclick="return confirm('Are you sure you want to save.');"name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								<span id="efrisGoodStatus"></span>
							</div>
							<script type="text/javascript">		
								var urlPath = $('#urlPath').val();
								$('#efrisGood').click(function(){
									$('#efrisGoodStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
									$(this).attr("disabled", true);
									var items = $('#items').val();
									if(1){
										//alert( $('#OperationType').val());
										var form_data = new FormData();				
										var form_data = new FormData();				
										form_data.append('OperationType', $('#OperationType').val());		
										form_data.append('goodCode', $('#goodCode').val());	
										form_data.append('goodID', $('#goodID').val());				
										form_data.append('goodName', $('#goodName').val());
										form_data.append('unitPrice', $('#unitPrice').val());
										form_data.append('uom', $('#uom').val());
										form_data.append('hasExciseTax', $('#hasExciseTax').val());
										form_data.append('categoryID', $('#categoryID').val());
										form_data.append('currency', $('#currency').val());
										form_data.append('stockWarning', $('#stockWarning').val());
										
										$.ajax({
											url: urlPath+"ajax/_efrisGood.php",
											type: "POST",
											data: form_data,
											contentType: false,
											cache: false,
											processData:false,
											success: function(data){			
												//alert(data);	
													//$('#efrisGoodStatus').append(data+"<br/><br/>");	
												var	values = JSON.parse(data);
												$('#efrisGood').attr("disabled",false);
												if(values.status == "error"){
													$('#efrisGoodStatus').html(values.error);
												}else{
													$('#efrisGoodStatus').html('Pushed');							
													$('#efrisGoodStatus').append(values.response);							
												}
											}
										});
									}else{
										//alert("Enter Lot No.");
										$('#efrisGoodStatus').html("");
									}
								});
							</script>
						</form>
					</div>
				</div>
			</div>
		</div>
		<?php		
	}
	
	public function createGoodAction(){
		$id = portion(3);
		$db = new Db();
		$sql = "SELECT * FROM efris_goods WHERE eg_id = '$id'";
		$select = $db->select($sql);
		extract($select[0][0]);

		echo '<b>Item code: </b>'.$eg_code;
		echo '<br/>';
		echo '<b>Name: </b>'.$eg_name;
?>
		<div class="" style="margin-top:10px;">
			<div class="panel panel-default">
				<div class="panel-heading">
					
				</div>
				<div class="panel-body">
					<div class="row">

						<form role="form" action="" method="post">
							<div class="col-md-12">
								<div id="must">All field with asterisk(*) are mandatory.</div>
							</div>
							<div class="clearfix"></div>

							<div class="col-lg-2">
								<div class="form-group">
									<label>OperationType<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="OperationType">
											<?php
											$ot = array('101'=>'Add Good', '102'=>'Modify Good');
											//$ot = array('101'=>'Add Good');
											foreach($ot as $o=>$t){
												if($o ==$eg_operation_type)
												echo '<option selected data-subtext="" value="'.$o.'">'.$t.'</option>';
												else
													echo '<option data-subtext="" value="'.$o.'">'.$t.'</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Good Code<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodCode" value="<?php echo @$eg_code; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-4">
								<div class="form-group">
									<label>Good Name<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="goodName" value="<?php echo @$eg_name; ?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Unit Price<span class="must"></span></label>
									<div class="form-line">
										<input class="form-control" type="text" autocomplete="off" id="unitPrice" value="<?php echo $eg_unit_price;?>">
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>UOM<span class="must">*</span></label>
									<div class="form-line">
										<select id="uom" style="width:100%;">
											<option value="">Select</option>
											<?php 
											$db = new Db();
											$sql = "SELECT * FROM dictionary WHERE d_category = '2' ORDER BY d_name ASC";
											$select = $db->select($sql);
											foreach($select[0] as $row){
												extract($row);
												if($eg_uom == $d_code)
													echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
													echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>
							<div class="clearfix"></div>
							<div class="col-lg-3">
								<div class="form-group">
									<label>Category Id<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" id="categoryID" value="<?php echo$eg_category_id;?>">
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Has excise TAX?<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="hasExciseTax">
											<?php
											$ot = array('101'=>'Yes', '102'=>'No');
											foreach($ot as $o=>$t){
												if($o ==$eg_has_excise_tax)
												echo '<option selected data-subtext="" value="'.$o.'">'.$t.'</option>';
												else
												echo '<option data-subtext="" value="'.$o.'">'.$t.'</option>';	
											}
											?>
										</select>
									</div>
								</div>
							</div>
							<div class="col-lg-2">
								<div class="form-group">
									<label>Currency<span class="must">*</span></label>
									<div class="form-line">
										<select data-show-subtext="true" data-live-search="true" style="width:100%" id="currency">
											<?php
											$db = new Db();
											$sql = "SELECT * FROM dictionary WHERE d_category = '1' ORDER BY d_name ASC";
											$select = $db->select($sql);
											foreach($select[0] as $row){
												extract($row);
												if($eg_currency == $d_code)
												echo '<option selected value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
												else
												echo '<option value="'.$d_code.'">'.$d_name.' ('.$d_code.')</option>';
											}
											?>
										</select>
									</div>
								</div>
							</div>	
							<div class="col-lg-2">
								<div class="form-group">
									<label>Stock Warning<span class="must">*</span></label>
									<div class="form-line">
										<input class="form-control" type="number" autocomplete="off" id="stockWarning" value="<?php echo$eg_stock_warning;?>">
									</div>
								</div>
							</div>						
							<input type="hidden" value="<?php echo $eg_id; ?>" id="goodID"/>
							<div class="clearfix"></div>
							<div class="col-lg-12">
								<button id="efrisGood" type="button" onclick="return confirm('Are you sure you want to save.');"name="submit" class="btn btn-primary"><i class="fa fa-fw fa-save"></i> Save</button>
								<span id="efrisGoodStatus"></span>
							</div>
							<script type="text/javascript">		
								var urlPath = $('#urlPath').val();
								$('#efrisGood').click(function(){
									$('#efrisGoodStatus').html('<img src="'+urlPath+'images/loading.gif" alt="loading..."/>');						
									$(this).attr("disabled", true);
									var items = $('#items').val();
									if(1){
										//alert( $('#OperationType').val());
										var form_data = new FormData();				
										var form_data = new FormData();				
										form_data.append('OperationType', $('#OperationType').val());		
										form_data.append('goodCode', $('#goodCode').val());	
										form_data.append('goodID', $('#goodID').val());				
										form_data.append('goodName', $('#goodName').val());
										form_data.append('unitPrice', $('#unitPrice').val());
										form_data.append('uom', $('#uom').val());
										form_data.append('hasExciseTax', $('#hasExciseTax').val());
										form_data.append('categoryID', $('#categoryID').val());
										form_data.append('currency', $('#currency').val());
										form_data.append('stockWarning', $('#stockWarning').val());
										
										$.ajax({
											url: urlPath+"ajax/_efrisGood.php",
											type: "POST",
											data: form_data,
											contentType: false,
											cache: false,
											processData:false,
											success: function(data){			
												//alert(data);	
													//$('#efrisGoodStatus').append(data+"<br/><br/>");	
												var	values = JSON.parse(data);
												$('#efrisGood').attr("disabled",false);
												if(values.status == "error"){
													$('#efrisGoodStatus').html(values.error);
												}else{
													$('#efrisGoodStatus').html('Pushed');							
													$('#efrisGoodStatus').append(values.response);							
												}
											}
										});
									}else{
										//alert("Enter Lot No.");
										$('#efrisGoodStatus').html("");
									}
								});
							</script>
						</form>
					</div>
				</div>
			</div>
		</div>
		<?php		
	}


	public function pending(){
		$db = new Db();
		$t = strtotime('2022-05-16');
		$sql = "SELECT * FROM efris_goods where eg_mapped IS NULL";
		$select = $db->select($sql);
		$num = "";
		if($db->num_rows())
			$num = $db->num_rows();
			
		return $num;
				
	}
		
}