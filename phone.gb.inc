<?php

/**
 * @file [2012-08-12]
 * CCK Field for British phone numbers.
 */
function phone_gb_metadata() {
  // These strings are translated using t() on output.
  return array(
    'label' => 'Phone Numbers - Great Britain - England (UK)',
    'error' => '"%value" is not a valid British (UK) phone number<br>British Phone numbers should contain 9 or 10 digits after<br>the 0 trunk code or +44 country code.',
  );
}

/*
    Accepts:
        020 7000 7000
        0207 000 7000
        02070 007 000 #4567
        0117 455 7777
        01174 557 777
        0175 061 8888
        01750 618 888
        01750 62555
        01697 355 555
        01697 73555
        02030007000
        02070008888
        01215557777
        01174007777
        01750615777
        0175062555
        0169773555
        01946755555
        07788555777
        07010001000
        08005557777
        0800555888
        (020) 7000 7000
        (0207) 000 7000
        (01174) 557 777 #333
        (01970) 234567 #0001
        (0169) 772444
        +44 207 000 7000
        +44 2070 007000
        +44 16973 55555
        +44 1697 355 555
        +441174 008 888 #333
        +44 203 000 8000 #555
        +441970234567
        +44(0)1970234567
        +44 1970 123 456
        +44 (0)1970 234 456
        +441174008888
        +441174008888#333
        +44 (0) 203 000 8000
        +44(1970)234567
        (+44) 1697 355 555
        (+44 16973) 55555 #888
        (+441970)234567
        (+44) 2070 007 000
        (+44 20) 3000 8000
        (+44) (0) 207 000 7000
        00 44 207 000 7000
        00 44 16977 3555
        00442030007777#555
        00 (44) 1697 73555
        (00 44) 203 000 8000
        011 44 207 000 7000
        011 (44) 20 7000 7000
        011 44 1697 73555
        011 44 (0)203 000 8000#222
    "Be liberal in what you accept and strict in what you return."

    Rejects:
        06750 618888
        06750 62555
        060 5000 7000
        00 33 20 00 70 00
        01 69 77 35 55
        05342030007777#555
        02230007777
        062300077
        06230007777
        0623000777788
        (0197) 0123456 #01
        +44 01970 123 456
*/

/**
 * Verifies that $phonenumber is a United Kingdom phone number in a valid
 * number range. Rejects numbers that are too long or too short or are in
 * a non-valid range. Accepts a wide range of input formats and a number
 * of different prefixes.
 * created by @g1smd
 *
 * @param string $phonenumber
 * @return boolean Returns boolean FALSE if the phone number is not valid
 */
function valid_gb_phone_number($phonenumber) {
    // Check if number entered matches a valid format
    if (!valid_gb_phone_pattern($phonenumber)) {
        return FALSE;
    } else {
        // Extract number parts: prefix, NSN, extension
        $phonenumberPartsArray = (extract_gb_phone_parts($phonenumber));
        if (empty($phonenumberPartsArray)) {
            return FALSE;
        } else {
            $phonenumberNSN = $phonenumberPartsArray['NSN'];
            // Check if NSN entered is in a valid range
            if (!valid_gb_phone_range($phonenumberNSN)) {
                return FALSE;
            } else {
                return TRUE;
            }
        }
    }
}

/**
 * Convert a valid United Kingdom phone number into standard +44 1970 123456 #001
 * international format. Accepts a wide range of input formats and prefixes and
 * re-formats the number taking into account the required 2+8, 3+7, 4+6, 4+5, 5+5,
 * 5+4 and 3+6 formats by number range.
 * created by @g1smd
 *
 * @param $phonenumber must be a valid nine or ten-digit number (with optional extension)
 * @return string $phonenumber
 */
function format_gb_phone_number($phonenumber, $field = FALSE) {
    $phonenumberPrefix = $phonenumberNSNraw = $phonenumberNSNformatted = $phonenumberExtension = '';

    // Extract optional country prefix, NSN, and optional extension.
    $phonenumberPartsArray = extract_gb_phone_parts($phonenumber);
    if (!empty($phonenumberPartsArray)) {
        $phonenumberNSN = $phonenumberPartsArray['NSN'];
        if ($phonenumberNSN == null) {
            return $phonenumber;
        }
        // Grab only the NSN part for formatting
        // NSN part might include spaces, hyphens or ')' and will need to be removed
        $arrayReplace = array(")", "-", " ");
        $phonenumberNSN = trim(str_replace($arrayReplace, "", $phonenumberNSN));
        // Format NSN part of GB number
        $phonenumberNSNformatted = format_gb_nsn($phonenumberNSN);

        // Set prefix as 44 or 0
        if (isset($phonenumberPartsArray['prefix']) && $phonenumberPartsArray['prefix'] != null) {
            $phonenumberPrefix = $phonenumberPartsArray['prefix'];
    //  } else {
    //      $phonenumberPrefix = "0";
        }

        // Extract extension
        $phonenumberHasExtension = false;
        $phonenumberExtension = null;
        if (isset($phonenumberPartsArray['extension']) && $phonenumberPartsArray['extension'] != null) {
            $phonenumberHasExtension = true;
            $phonenumberExtension = " " . trim($phonenumberPartsArray['extension']);
        }

        // Add prefix back on to NSN
        if ($field['phone_country_code']) {
            // Use formatting from form selector
            $phonenumber = '+44 ' . $phonenumberNSNformatted;
        } else {
            // Use formatting from type-in
            $phonenumber = $phonenumberPrefix . $phonenumberNSNformatted;
        }

        // Add extension back on to number
        if ($phonenumberHasExtension) {
            $phonenumber .= $phonenumberExtension;
        }
    }
    return $phonenumber;
}

/**
 * Verifies that $phonenumber uses a valid UK phone number input pattern.
 * Pattern matches any number entered as 2+8, 3+7, 4+6, 4+5, 5+5, 5+4, 3+6
 * with or without spaces, with a variety of prefixes and optional extension.
 * RegEx patterns are based on
 * http://www.aa-asterisk.org.uk/index.php/Number_format and
 * http://www.aa-asterisk.org.uk/index.php/Regular_Expressions_for_Validating_and_Formatting_UK_Telephone_Numbers
 * created by @g1smd
 *
 * Regular expression is adapted from Amos Hurd's RegEx at RegExLib.com
 * Reformulated by @g1smd to remove multiple inefficiencies in pattern and 
 * include various valid number formats that the original pattern omitted.
 *
 * @param string $phonenumber
 * @return boolean Returns boolean FALSE if the phone number is not valid.
 */
function valid_gb_phone_pattern($phonenumber) {
    // RegEx to define valid formats for GB numbers
    $patternRegExGB  = '/^';
    $patternRegExGB .= '(?:';
        $patternRegExGB .= '(?:\(?(?:0(?:0|11)\)?\s?\(?|\+)44\)?\s?(?:\(?0\)?\s?)?)';  # leading 00, 011 or + before 44 with optional (0); parentheses and spaces optional
        $patternRegExGB .= '|';
        $patternRegExGB .= '(?:\(?0)';                                  # leading (0, 0
    $patternRegExGB .= ')';
    $patternRegExGB .= '(?:';
        $patternRegExGB .= '(?:\d{5}\)?\s?\d{4,5})';                    # [5+4]/[5+5]
        $patternRegExGB .= '|';
        $patternRegExGB .= '(?:\d{4}\)?\s?(?:\d{5}|\d{3}\s?\d{3}))';    # [4+5]/[4+6]
        $patternRegExGB .= '|';
        $patternRegExGB .= '(?:\d{3}\)?\s?\d{3}\s?\d{3,4})';            # [3+6]/[3+7]
        $patternRegExGB .= '|';
        $patternRegExGB .= '(?:\d{2}\)?\s?\d{4}\s?\d{4})';              # [2+8]
    $patternRegExGB .= ')';
    $patternRegExGB .= '(?:';
        $patternRegExGB .= '(\s?\#\d+)?';                               # optional extension number shown with a hash divider
    $patternRegExGB .= ')';
    $patternRegExGB .= '$/x';

// Test number entered for matching format
    if (!preg_match($patternRegExGB, $phonenumber)) {
        return FALSE;
    } else {
        return TRUE;
    }
}

/**
 * Extract parts from GB phone number: prefix, NSN and optional extension.
 * Accepts a wide range of input formats and prefixes. This function also
 * cleans up the NSN part by removing spaces, hyphens and brackets. Returned
 * prefix is either +44 with space or a 0 without space.
 * created by @g1smd
 *
 * @param string $phonenumber must be a valid UK phone number (with optional extension)
 * @return array $phonenumberPartsArray Returns prefix, NSN and extension in array.
 */
function extract_gb_phone_parts($phonenumber) {
    // RegEx to extract number parts: 44 prefix ($2), NSN ($3), extension ($4)
    $patternGBnumberparts  = '/^';
    $patternGBnumberparts .= '(\(?(?:0(?:0|11)\)?\s?\(?|\+)(44)\)?\s?)?\(?0?(?:\)\s?)?';    # country or trunk prefix
    $patternGBnumberparts .= '(';
        $patternGBnumberparts .= '[1-9]\d{1,4}\)?[\d\s]+';                                  # NSN
    $patternGBnumberparts .= ')';
    $patternGBnumberparts .= '(\#\d+)?';                                                    # optional extension
    $patternGBnumberparts .= '$/x';
    if (preg_match($patternGBnumberparts, $phonenumber, $matches)) {

        // Extract NSN part of GB number
        if (ISSET($matches['3'])) {
            $phonenumberNSNraw = $matches['3'];
            // Trim NSN and remove space, hyphen or ')' if present
            $arrayReplace = array(")", "-", " ");
            $phonenumberNSN = trim(str_replace($arrayReplace, "", $phonenumberNSNraw));

            // Extract 44 prefix if present and set prefix as 0 or as +44 and space
            if (ISSET($matches['2']) && $matches['2'] == '44') {
                $phonenumberPrefix = '+44 ';
            } else {
                $phonenumberPrefix = '0';
            }

            // Extract extension
            $phonenumberExtension = null;
            if (ISSET($matches['4'])) {
                $phonenumberExtension = ' ' . $matches['4'];
            }
        }
//  } else {
    }

    $phonenumberPartsArray = array(
            'NSN'       => $phonenumberNSN,
            'prefix'    => $phonenumberPrefix,
            'extension' => $phonenumberExtension,
        );

    return $phonenumberPartsArray;
}

/**
 * Verifies that $phonenumberNSN is a valid UK phone number range by initial
 * digits and length. Tests the NSN part for length and number range. Based on
 * http://www.aa-asterisk.org.uk/index.php/Number_format and
 * http://www.aa-asterisk.org.uk/index.php/Regular_Expressions_for_Validating_and_Formatting_UK_Telephone_Numbers
 * created by @g1smd
 * 
 * @param string $phonenumberNSN
 * @return boolean Returns boolean FALSE if the phone number is not valid.
 */
function valid_gb_phone_range($phonenumberNSN) {
    // RegEx to define valid ranges for NSN by initial digits and length
    $patternGBvalidrange  = '/^';
    $patternGBvalidrange .= '(';
        $patternGBvalidrange .= '(1[1-9]|2[03489]|3[0347]|5[56]|7[04-9]|8[047]|9[018])\d{8}';
        $patternGBvalidrange .= '|';
        $patternGBvalidrange .= '(1[2-9]\d|[58]00)\d{6}';
        $patternGBvalidrange .= '|';
        $patternGBvalidrange .= '8(001111|45464\d)';
    $patternGBvalidrange .= ')';
    $patternGBvalidrange .= '$/x';
    // Test NSN to see if it matches a valid number range
    if (preg_match($patternGBvalidrange, $phonenumberNSN)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * Format GB phone numbers in correct format per number range. Based on
 * http://www.aa-asterisk.org.uk/index.php/Number_format and
 * http://www.aa-asterisk.org.uk/index.php/Regular_Expressions_for_Validating_and_Formatting_UK_Telephone_Numbers
 * created by @g1smd
 *
 * @param string $phonenumberNSN Must be the 10 or 9 digit NSN part of the number.
 * @return string $phonenumberNSN Returns correctly formatted NSN by length and range.
 */
function format_gb_nsn($phonenumberNSN) {
    // Trim NSN
    $phonenumberNSN = trim($phonenumberNSN);
    // Find NSN string length
    $phonenumberNSNLength = strlen($phonenumberNSN);
    // RegEx patterns to define formatting by length and initial digits
    // [2+8] 2d, 55, 56, 70, 76 (not 7624)
    $pattern28 = '/^(?:2|5[56]|7(?:0|6(?:[013-9]|2[0-35-9])))/';
    $capture28 = '/^(\d{2})(\d{4})(\d{4})$/';
    // [3+7] 11d, 1d1, 3dd, 80d, 84d, 87d, 9dd
    $pattern37 = '/^(?:1(?:1|\d1)|3|8(?:0[08]|4[2-5]|7[0-3])|9[018])/';
    $capture37 = '/^(\d{3})(\d{3})(\d{4})$/';
    // [5+5] 1dddd (12 areas)
    $pattern55 = '/^(?:1(?:3873|5(?:242|39[456])|697[347]|768[347]|9467))/';
    $capture55 = '/^(\d{5})(\d{5})$/';
    // [5+4] 1ddd (1 area)
    $pattern54 = '/^(?:16977[23])/';
    $capture54 = '/^(\d{5})(\d{4})$/';
    // [4+6] 1ddd, 7ddd (inc 7624) (not 70, 76)
    $pattern46 = '/^(?:1|7(?:[1-5789]|624))/';
    $capture46 = '/^(\d{4})(\d{6})$/';
    // [4+5] 1ddd (40 areas)
    $pattern45 = '/^(?:1(?:2(?:0[48]|54|76|9[78])|3(?:6[34]|8[46])|4(?:04|20|6[01]|8[08])|5(?:27|6[26])|6(?:06|29|35|47|59|95)|7(?:26|44|50)|8(?:27|37|84)|9(?:0[05]|35|49|63|95)))/';
    $capture45 = '/^(\d{4})(\d{5})$/';
    // [3+6] 500, 800
    $pattern36 = '/^([58]00)/';
    $capture36 = '/^(\d{3})(\d{6})$/';
    // Format numbers by leading digits and length
    if ($phonenumberNSNLength == 10 && preg_match($pattern28, $phonenumberNSN)) {
        if (preg_match($capture28, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'] . " " . $matches['3'];
        }
    } else if ($phonenumberNSNLength == 10 && preg_match($pattern37, $phonenumberNSN)) {
        if (preg_match($capture37, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'] . " " . $matches['3'];
        }
    } else if ($phonenumberNSNLength == 10 && preg_match($pattern55, $phonenumberNSN)) {
        if (preg_match($capture55, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'];
        }
    } else if ($phonenumberNSNLength == 9  && preg_match($pattern54, $phonenumberNSN)) {
        if (preg_match($capture54, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'];
        }
    } else if ($phonenumberNSNLength == 10 && preg_match($pattern46, $phonenumberNSN)) {
        if (preg_match($capture46, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'];
        }
    } else if ($phonenumberNSNLength == 9  && preg_match($pattern45, $phonenumberNSN)) {
        if (preg_match($capture45, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'];
        }
    } else if ($phonenumberNSNLength == 9  && preg_match($pattern36, $phonenumberNSN)) {
        if (preg_match($capture36, $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'];
        }
    } else if ($phonenumberNSNLength > 5) {
        // Default format for non-valid numbers (shouldn't ever get here)
        if (preg_match("/^(\d)(\d{4})(\d*)$/", $phonenumberNSN, $matches)) {
            $phonenumberNSN = $matches['1'] . " " . $matches['2'] . " " . $matches['3'];
        }
    }
    return $phonenumberNSN;
}

?>
