<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {

    // Trim and collect input
    function clean($str) {
        return htmlspecialchars(trim($str));
    }

    $fdn = clean($_POST['fdn'] ?? '');
    $type = clean($_POST['transaction_type'] ?? '');
    $year = intval($_POST['year'] ?? 0);
    $month = intval($_POST['month'] ?? 0);
    $generator = intval($_POST['generator'] ?? 0);
    $date = clean($_POST['date'] ?? '');
    $exchangeRate = floatval($_POST['exchange_rate'] ?? 0);
    $deemedEnergy = floatval($_POST['deemed_energy'] ?? 0);
    $qty = floatval($_POST['qty'] ?? 0);
    $vat = floatval($_POST['vat'] ?? 0);
    $amount = floatval($_POST['amount'] ?? 0);
    $total = floatval($_POST['total'] ?? 0);

    // Validation
    $errors = [];

    if (!$fdn) $errors[] = "FDN is required.";
    if (!in_array($type, ['INVOICE', 'CREDIT NOTE', 'DEBIT NOTE'])) $errors[] = "Invalid transaction type.";
    if ($year < 2019 || $year > intval(date('Y'))) $errors[] = "Invalid year.";
    if ($month < 1 || $month > 12) $errors[] = "Invalid month.";
    if ($generator <= 0) $errors[] = "Invalid generator.";
    if (!$date) $errors[] = "Date is required.";
    if ($exchangeRate <= 0) $errors[] = "Exchange rate must be positive.";
    if ($deemedEnergy < 0) $errors[] = "Deemed energy must be numeric.";
    if ($qty <= 0) $errors[] = "Quantity must be positive.";
    if ($vat < 0) $errors[] = "VAT must be numeric.";
    if ($amount <= 0) $errors[] = "Amount must be positive.";
    if ($total <= 0) $errors[] = "Total must be positive.";

    if (!empty($errors)) {
        echo "Validation failed:\n" . implode("\n", $errors);
        exit;
    }

    // TODO: Save to database (use prepared statements!)

    echo "Transaction saved successfully.";
}
