<?php

/**
 * @file [2012-08-12]
 * Test data for CCK Phone GB.
 */

include('phone.gb.inc');


// TEST DATA

print '<hr>';
$msgarray = phone_gb_metadata();
print '<font size="3">';
print ' <b>00 - msgarray</b> - [' . $msgarray['label'] . '] <br>';
print ' <b>00 - msgarray</b> - [' . $msgarray['error'] . '] <hr>';
print '<hr>';


$phonenumberForTesting = array(
        '020 5000 7000',
        '0205 000 7000',
        '0208 000 7000',
        '02050 007 000 #4567',
        '0117 455 7777',
        '01174 557 777',
        '01177 557 777',
        '0175 061 8888',
        '01750 618 888',
        '01750 62555',
        '01750 61555',
        '01697 355 555',
        '01697 73555',
        '0800777888',
        '08005558888',
        '02030007000',
        '01215557777',
        '02050008888',
        '0175062555',
        '0169773555',
        '0500777888',
        '05005558888',
        '07005558888',
        '(020) 7000 7000',
        '(0207) 000 7000',
        '(01174) 557 777 #333',
        '(0169) 772444',
        '+44 207 000 7000',
        '+44 2050 007000',
        '+44 2070 007000',
        '+44 16973 55555',
        '+44 1697 355 555',
        '+441174 008 888 #333',
        '+44 203 000 8000 #555',
        '+441177008888',
        '+441174008888',
        '+441174008888#333',
        '+44 (0) 203 000 8000',
        '(+44) 1697 355 555',
        '(+44 16973) 55555 #888',
        '(+44) 2050 007 000',
        '(+44 20) 3000 8000',
        '(+44) (0) 205 000 7000',
        '00 44 205 000 7000',
        '00 44 16977 3555',
        '00442030007777#555',
        '00 (44) 1697 73555',
        '(00 44) 203 000 8000',
        '011 44 205 000 7000',
        '011 (44) 20 5000 7000',
        '011 (44) (0)205 000 7000',
        '011 44 1697 73555',
        '011 44 (0)203 000 8000#222',
        '06750 618888',
        '06750 62555',
        '060 5000 7000',
        '00 33 20 00 70 00',
        '01 69 77 35 55',
        '05342030007777#555',
    );
$maxcount = sizeof($phonenumberForTesting);
print "COUNT - " . $maxcount . " <hr><hr>";


for ($i = 0; $i < $maxcount; $i++) {
    print ' <b>Type 10 - Test '. $i . ' - ' . $phonenumberForTesting[$i] . ' - valid_gb_phone_number</b> - [' . valid_gb_phone_number($phonenumberForTesting[$i]) . '] <hr> ';
}
print '<hr>';


for ($i = 0; $i < $maxcount; $i++) {
    print ' <b>Type 20 - Test '. $i . ' - ' . $phonenumberForTesting[$i] . ' - format_gb_phone_number</b> - [' . format_gb_phone_number($phonenumberForTesting[$i]) . '] <hr> ';
}
print '<hr>';


for ($i = 0; $i < $maxcount; $i++) {
    print ' <b>Type 30 - Test '. $i . ' - ' . $phonenumberForTesting[$i] . ' - valid_gb_phone_pattern</b> - [' . valid_gb_phone_pattern($phonenumberForTesting[$i]) . '] <hr> ';
}
print '<hr>';


for ($i = 0; $i < $maxcount; $i++) {
    $return = extract_gb_phone_parts($phonenumberForTesting[$i]);
    print ' <b>Type 40 - Test '. $i . ' - ' . $phonenumberForTesting[$i] . ' - extract_gb_phone_parts</b> - [' . $return . '] <pre>' ; var_dump($return); print '</pre> <hr> ';
}
print '<hr>';



$NSNForTesting = array(
        '2030007777',
        '1174007777',
        '1750615777',
        '1946777777',
        '1750615555',
        '175062555',
        '1946755555',
        '169773555',
        '500777555',
        '5008887777',
        '7788555777',
        '7010001000',
        '8005557777',
        '800555888',
        '5007778888',
        '2230007777',
        '62300077',
        '6230007777',
        '623000777788',
    );
$maxcount = sizeof($NSNForTesting);
print "COUNT - " . $maxcount . " <hr><hr>";


for ($i = 0; $i < $maxcount; $i++) {
    print ' <b>Type 50 - Test ' . $i . ' - ' . $NSNForTesting[$i] . ' - valid_gb_phone_range</b> - [' . valid_gb_phone_range($NSNForTesting[$i]) . '] <hr> ';
}
print '<hr>';


for ($i = 0; $i < $maxcount; $i++) {
    print ' <b>Type 60 - Test '. $i . ' - ' . $NSNForTesting[$i] . ' - format_gb_nsn</b> - [' . format_gb_nsn($NSNForTesting[$i]) . '] <hr> ';
}
print '<hr><hr>';

// TEST DATA - END


?>
