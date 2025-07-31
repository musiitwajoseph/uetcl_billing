<?php

include "../classes/init.inc";

$t = new BeforeAndAfter();


$id = $_POST['id'];

$db = new Db();
$select = $db->select("SELECT * FROM other_invoice WHERE oi_id='$id'");

if($db->num_rows()){
    extract($select[0][0]);
}

echo '<pre>';
print_r($select);
echo '</pre>';

  // define("JAVA_HOSTS", "localhost:6996");
  // define("JAVA_SERVLET", "/UETCL/TCL64LENAPI");
  // require_once("java/Java.inc");

  //echo java_context()->getServlet()->hello();
  //echo java_context()->getServlet()->sun("Kasule Rogers");
  
  $str = <<<'EOD'
<SSC>
<User>
<Name>SSC</Name>
</User>
<SunSystemsContext>
<BusinessUnit>TCO</BusinessUnit>
<BudgetCode>A</BudgetCode>
</SunSystemsContext>
<MethodContext>
<LedgerPostingParameters>
<AllowBalTran></AllowBalTran>
<AllowOverBudget></AllowOverBudget>
<AllowPostToSuspended></AllowPostToSuspended>
<BalancingOptions></BalancingOptions>
<DefaultPeriod></DefaultPeriod>
<Description>Testing SSC Ledger Import</Description>
<JournalType>SINV</JournalType>
<LayoutCode>LIALL</LayoutCode>
<LoadOnly>N</LoadOnly>
<PostProvisional></PostProvisional>
<PostToHold>Y</PostToHold>
<PostingType></PostingType>
<Print></Print>
<ReportErrorsOnly>Y</ReportErrorsOnly>
<ReportingAccount>99998</ReportingAccount>
<SuppressSubstitutedMessages>Y</SuppressSubstitutedMessages>
<SuspenseAccount>99998</SuspenseAccount>
<TransactionAmountAccount>99998</TransactionAmountAccount>
</LedgerPostingParameters>
</MethodContext>
<Payload>
<Ledger>
<Line>
<AccountCode>300004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>C</DebitCredit>
<Description>Testing SSC Integration-energy sales</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>500000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>150004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>D</DebitCredit>
<Description>Testing SSC Integration-ENERGY SALES</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>500000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>300004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>C</DebitCredit>
<Description>Testing SSC Integration-energy sales</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>230000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>150004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>D</DebitCredit>
<Description>Testing SSC Integration-ENERGY SALES</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>230000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>300004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>C</DebitCredit>
<Description>Testing SSC Integration-energy sales</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>740000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>150004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>D</DebitCredit>
<Description>Testing SSC Integration-ENERGY SALES</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>740000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>300004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>C</DebitCredit>
<Description>Testing SSC Integration-energy sales</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>500000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>150004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>D</DebitCredit>
<Description>Testing SSC Integration-ENERGY SALES</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>500000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>300004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>C</DebitCredit>
<Description>Testing SSC Integration-energy sales</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>230000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>150004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>D</DebitCredit>
<Description>Testing SSC Integration-ENERGY SALES</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>230000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>300004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>C</DebitCredit>
<Description>Testing SSC Integration-energy sales</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>740000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
<Line>
<AccountCode>150004</AccountCode>
<AccountingPeriod>2021003</AccountingPeriod>
<AnalysisCode1></AnalysisCode1>
<AnalysisCode10></AnalysisCode10>
<AnalysisCode2></AnalysisCode2>
<AnalysisCode3></AnalysisCode3>
<AnalysisCode4></AnalysisCode4>
<AnalysisCode5></AnalysisCode5>
<AnalysisCode6></AnalysisCode6>
<AnalysisCode7></AnalysisCode7>
<AnalysisCode8></AnalysisCode8>
<AnalysisCode9></AnalysisCode9>
<BaseAmount></BaseAmount>
<ConversionRate></ConversionRate>
<CurrencyCode>USD</CurrencyCode>
<DebitCredit>D</DebitCredit>
<Description>Testing SSC Integration-ENERGY SALES</Description>
<JournalSource>SSC</JournalSource>
<JournalType>SINV</JournalType>
<TransactionAmount>740000</TransactionAmount>
<TransactionDate>19092020</TransactionDate>
<TransactionReference>3004</TransactionReference>
</Line>
</Ledger>
</Payload>
</SSC>
EOD;

echo '<pre>';
echo nl2br(htmlspecialchars($str));
echo '</pre>';


//echo java_context()->getServlet()->SendToSun(array($str));

?>