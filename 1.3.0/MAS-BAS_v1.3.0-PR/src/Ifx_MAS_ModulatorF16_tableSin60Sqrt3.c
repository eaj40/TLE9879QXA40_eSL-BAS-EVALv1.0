/*
 * Copyright (c) 2022 Infineon Technologies AG. All Rights Reserved.
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG, its affiliates or its licensees. If and as long as no
 * such terms of use are agreed, use of this file is subject to the Evaluation Software License Agreement distributed
 * along with this file within the software delivery package.
 *
 */

#include "Ifx_MAS_ModulatorF16.h"

/**
 * Lookup table to store the values needed to calculate the switching times.
 *
 * Values: sqrt(3) * sin(x/4096 * 60deg), x = 0,1,...,4095, in signed integer q14 format.[0 ... 1.4965]
 */
const Ifx_Math_Fract16 Ifx_MAS_ModulatorF16_lutSin60Sqrt3[4096] = {
    0, 7, 15, 22, 29, 36, 44, 51, 58, 65, 73, 80, 87, 94, 102, 109, 116, 123, 131, 138, 145, 152, 160, 167, 174, 181,
    189, 196, 203, 210, 218, 225, 232, 239, 247, 254, 261, 269, 276, 283, 290, 298, 305, 312, 319, 327, 334, 341, 348,
    356, 363, 370, 377, 385, 392, 399, 406, 414, 421, 428, 435, 443, 450, 457, 464, 472, 479, 486, 493, 501, 508, 515,
    522, 530, 537, 544, 551, 559, 566, 573, 581, 588, 595, 602, 610, 617, 624, 631, 639, 646, 653, 660, 668, 675, 682,
    689, 697, 704, 711, 718, 726, 733, 740, 747, 755, 762, 769, 776, 784, 791, 798, 805, 813, 820, 827, 834, 842, 849,
    856, 863, 871, 878, 885, 892, 900, 907, 914, 921, 929, 936, 943, 950, 958, 965, 972, 979, 987, 994, 1001, 1009,
    1016, 1023, 1030, 1038, 1045, 1052, 1059, 1067, 1074, 1081, 1088, 1096, 1103, 1110, 1117, 1125, 1132, 1139, 1146,
    1154, 1161, 1168, 1175, 1183, 1190, 1197, 1204, 1212, 1219, 1226, 1233, 1241, 1248, 1255, 1262, 1270, 1277, 1284,
    1291, 1299, 1306, 1313, 1320, 1328, 1335, 1342, 1349, 1357, 1364, 1371, 1378, 1386, 1393, 1400, 1407, 1415, 1422,
    1429, 1436, 1444, 1451, 1458, 1465, 1473, 1480, 1487, 1494, 1501, 1509, 1516, 1523, 1530, 1538, 1545, 1552, 1559,
    1567, 1574, 1581, 1588, 1596, 1603, 1610, 1617, 1625, 1632, 1639, 1646, 1654, 1661, 1668, 1675, 1683, 1690, 1697,
    1704, 1712, 1719, 1726, 1733, 1741, 1748, 1755, 1762, 1770, 1777, 1784, 1791, 1799, 1806, 1813, 1820, 1827, 1835,
    1842, 1849, 1856, 1864, 1871, 1878, 1885, 1893, 1900, 1907, 1914, 1922, 1929, 1936, 1943, 1951, 1958, 1965, 1972,
    1980, 1987, 1994, 2001, 2008, 2016, 2023, 2030, 2037, 2045, 2052, 2059, 2066, 2074, 2081, 2088, 2095, 2103, 2110,
    2117, 2124, 2132, 2139, 2146, 2153, 2160, 2168, 2175, 2182, 2189, 2197, 2204, 2211, 2218, 2226, 2233, 2240, 2247,
    2255, 2262, 2269, 2276, 2283, 2291, 2298, 2305, 2312, 2320, 2327, 2334, 2341, 2349, 2356, 2363, 2370, 2377, 2385,
    2392, 2399, 2406, 2414, 2421, 2428, 2435, 2443, 2450, 2457, 2464, 2471, 2479, 2486, 2493, 2500, 2508, 2515, 2522,
    2529, 2537, 2544, 2551, 2558, 2565, 2573, 2580, 2587, 2594, 2602, 2609, 2616, 2623, 2630, 2638, 2645, 2652, 2659,
    2667, 2674, 2681, 2688, 2696, 2703, 2710, 2717, 2724, 2732, 2739, 2746, 2753, 2761, 2768, 2775, 2782, 2789, 2797,
    2804, 2811, 2818, 2826, 2833, 2840, 2847, 2854, 2862, 2869, 2876, 2883, 2891, 2898, 2905, 2912, 2919, 2927, 2934,
    2941, 2948, 2955, 2963, 2970, 2977, 2984, 2992, 2999, 3006, 3013, 3020, 3028, 3035, 3042, 3049, 3057, 3064, 3071,
    3078, 3085, 3093, 3100, 3107, 3114, 3121, 3129, 3136, 3143, 3150, 3157, 3165, 3172, 3179, 3186, 3194, 3201, 3208,
    3215, 3222, 3230, 3237, 3244, 3251, 3258, 3266, 3273, 3280, 3287, 3294, 3302, 3309, 3316, 3323, 3331, 3338, 3345,
    3352, 3359, 3367, 3374, 3381, 3388, 3395, 3403, 3410, 3417, 3424, 3431, 3439, 3446, 3453, 3460, 3467, 3475, 3482,
    3489, 3496, 3503, 3511, 3518, 3525, 3532, 3539, 3547, 3554, 3561, 3568, 3575, 3583, 3590, 3597, 3604, 3611, 3619,
    3626, 3633, 3640, 3647, 3655, 3662, 3669, 3676, 3683, 3691, 3698, 3705, 3712, 3719, 3727, 3734, 3741, 3748, 3755,
    3763, 3770, 3777, 3784, 3791, 3798, 3806, 3813, 3820, 3827, 3834, 3842, 3849, 3856, 3863, 3870, 3878, 3885, 3892,
    3899, 3906, 3914, 3921, 3928, 3935, 3942, 3949, 3957, 3964, 3971, 3978, 3985, 3993, 4000, 4007, 4014, 4021, 4028,
    4036, 4043, 4050, 4057, 4064, 4072, 4079, 4086, 4093, 4100, 4107, 4115, 4122, 4129, 4136, 4143, 4151, 4158, 4165,
    4172, 4179, 4186, 4194, 4201, 4208, 4215, 4222, 4230, 4237, 4244, 4251, 4258, 4265, 4273, 4280, 4287, 4294, 4301,
    4308, 4316, 4323, 4330, 4337, 4344, 4351, 4359, 4366, 4373, 4380, 4387, 4394, 4402, 4409, 4416, 4423, 4430, 4437,
    4445, 4452, 4459, 4466, 4473, 4480, 4488, 4495, 4502, 4509, 4516, 4523, 4531, 4538, 4545, 4552, 4559, 4566, 4574,
    4581, 4588, 4595, 4602, 4609, 4617, 4624, 4631, 4638, 4645, 4652, 4660, 4667, 4674, 4681, 4688, 4695, 4702, 4710,
    4717, 4724, 4731, 4738, 4745, 4753, 4760, 4767, 4774, 4781, 4788, 4796, 4803, 4810, 4817, 4824, 4831, 4838, 4846,
    4853, 4860, 4867, 4874, 4881, 4888, 4896, 4903, 4910, 4917, 4924, 4931, 4938, 4946, 4953, 4960, 4967, 4974, 4981,
    4989, 4996, 5003, 5010, 5017, 5024, 5031, 5039, 5046, 5053, 5060, 5067, 5074, 5081, 5088, 5096, 5103, 5110, 5117,
    5124, 5131, 5138, 5146, 5153, 5160, 5167, 5174, 5181, 5188, 5196, 5203, 5210, 5217, 5224, 5231, 5238, 5245, 5253,
    5260, 5267, 5274, 5281, 5288, 5295, 5303, 5310, 5317, 5324, 5331, 5338, 5345, 5352, 5360, 5367, 5374, 5381, 5388,
    5395, 5402, 5409, 5417, 5424, 5431, 5438, 5445, 5452, 5459, 5466, 5474, 5481, 5488, 5495, 5502, 5509, 5516, 5523,
    5530, 5538, 5545, 5552, 5559, 5566, 5573, 5580, 5587, 5595, 5602, 5609, 5616, 5623, 5630, 5637, 5644, 5651, 5659,
    5666, 5673, 5680, 5687, 5694, 5701, 5708, 5715, 5723, 5730, 5737, 5744, 5751, 5758, 5765, 5772, 5779, 5786, 5794,
    5801, 5808, 5815, 5822, 5829, 5836, 5843, 5850, 5858, 5865, 5872, 5879, 5886, 5893, 5900, 5907, 5914, 5921, 5928,
    5936, 5943, 5950, 5957, 5964, 5971, 5978, 5985, 5992, 5999, 6007, 6014, 6021, 6028, 6035, 6042, 6049, 6056, 6063,
    6070, 6077, 6085, 6092, 6099, 6106, 6113, 6120, 6127, 6134, 6141, 6148, 6155, 6162, 6170, 6177, 6184, 6191, 6198,
    6205, 6212, 6219, 6226, 6233, 6240, 6247, 6255, 6262, 6269, 6276, 6283, 6290, 6297, 6304, 6311, 6318, 6325, 6332,
    6339, 6347, 6354, 6361, 6368, 6375, 6382, 6389, 6396, 6403, 6410, 6417, 6424, 6431, 6438, 6445, 6453, 6460, 6467,
    6474, 6481, 6488, 6495, 6502, 6509, 6516, 6523, 6530, 6537, 6544, 6551, 6559, 6566, 6573, 6580, 6587, 6594, 6601,
    6608, 6615, 6622, 6629, 6636, 6643, 6650, 6657, 6664, 6671, 6678, 6686, 6693, 6700, 6707, 6714, 6721, 6728, 6735,
    6742, 6749, 6756, 6763, 6770, 6777, 6784, 6791, 6798, 6805, 6812, 6819, 6827, 6834, 6841, 6848, 6855, 6862, 6869,
    6876, 6883, 6890, 6897, 6904, 6911, 6918, 6925, 6932, 6939, 6946, 6953, 6960, 6967, 6974, 6981, 6988, 6995, 7002,
    7009, 7017, 7024, 7031, 7038, 7045, 7052, 7059, 7066, 7073, 7080, 7087, 7094, 7101, 7108, 7115, 7122, 7129, 7136,
    7143, 7150, 7157, 7164, 7171, 7178, 7185, 7192, 7199, 7206, 7213, 7220, 7227, 7234, 7241, 7248, 7255, 7262, 7269,
    7276, 7283, 7290, 7297, 7304, 7311, 7318, 7325, 7332, 7339, 7346, 7354, 7361, 7368, 7375, 7382, 7389, 7396, 7403,
    7410, 7417, 7424, 7431, 7438, 7445, 7452, 7459, 7466, 7473, 7480, 7487, 7494, 7501, 7508, 7515, 7522, 7529, 7536,
    7543, 7550, 7557, 7564, 7571, 7578, 7585, 7592, 7599, 7606, 7613, 7620, 7626, 7633, 7640, 7647, 7654, 7661, 7668,
    7675, 7682, 7689, 7696, 7703, 7710, 7717, 7724, 7731, 7738, 7745, 7752, 7759, 7766, 7773, 7780, 7787, 7794, 7801,
    7808, 7815, 7822, 7829, 7836, 7843, 7850, 7857, 7864, 7871, 7878, 7885, 7892, 7899, 7906, 7913, 7920, 7927, 7934,
    7941, 7947, 7954, 7961, 7968, 7975, 7982, 7989, 7996, 8003, 8010, 8017, 8024, 8031, 8038, 8045, 8052, 8059, 8066,
    8073, 8080, 8087, 8094, 8101, 8108, 8115, 8121, 8128, 8135, 8142, 8149, 8156, 8163, 8170, 8177, 8184, 8191, 8198,
    8205, 8212, 8219, 8226, 8233, 8240, 8247, 8254, 8260, 8267, 8274, 8281, 8288, 8295, 8302, 8309, 8316, 8323, 8330,
    8337, 8344, 8351, 8358, 8365, 8371, 8378, 8385, 8392, 8399, 8406, 8413, 8420, 8427, 8434, 8441, 8448, 8455, 8462,
    8468, 8475, 8482, 8489, 8496, 8503, 8510, 8517, 8524, 8531, 8538, 8545, 8552, 8558, 8565, 8572, 8579, 8586, 8593,
    8600, 8607, 8614, 8621, 8628, 8635, 8641, 8648, 8655, 8662, 8669, 8676, 8683, 8690, 8697, 8704, 8711, 8717, 8724,
    8731, 8738, 8745, 8752, 8759, 8766, 8773, 8780, 8787, 8793, 8800, 8807, 8814, 8821, 8828, 8835, 8842, 8849, 8855,
    8862, 8869, 8876, 8883, 8890, 8897, 8904, 8911, 8918, 8924, 8931, 8938, 8945, 8952, 8959, 8966, 8973, 8979, 8986,
    8993, 9000, 9007, 9014, 9021, 9028, 9035, 9041, 9048, 9055, 9062, 9069, 9076, 9083, 9090, 9096, 9103, 9110, 9117,
    9124, 9131, 9138, 9145, 9151, 9158, 9165, 9172, 9179, 9186, 9193, 9199, 9206, 9213, 9220, 9227, 9234, 9241, 9248,
    9254, 9261, 9268, 9275, 9282, 9289, 9296, 9302, 9309, 9316, 9323, 9330, 9337, 9344, 9350, 9357, 9364, 9371, 9378,
    9385, 9391, 9398, 9405, 9412, 9419, 9426, 9433, 9439, 9446, 9453, 9460, 9467, 9474, 9480, 9487, 9494, 9501, 9508,
    9515, 9521, 9528, 9535, 9542, 9549, 9556, 9562, 9569, 9576, 9583, 9590, 9597, 9603, 9610, 9617, 9624, 9631, 9638,
    9644, 9651, 9658, 9665, 9672, 9679, 9685, 9692, 9699, 9706, 9713, 9719, 9726, 9733, 9740, 9747, 9754, 9760, 9767,
    9774, 9781, 9788, 9794, 9801, 9808, 9815, 9822, 9828, 9835, 9842, 9849, 9856, 9862, 9869, 9876, 9883, 9890, 9897,
    9903, 9910, 9917, 9924, 9931, 9937, 9944, 9951, 9958, 9964, 9971, 9978, 9985, 9992, 9998, 10005, 10012, 10019,
    10026, 10032, 10039, 10046, 10053, 10060, 10066, 10073, 10080, 10087, 10093, 10100, 10107, 10114, 10121, 10127,
    10134, 10141, 10148, 10154, 10161, 10168, 10175, 10182, 10188, 10195, 10202, 10209, 10215, 10222, 10229, 10236,
    10243, 10249, 10256, 10263, 10270, 10276, 10283, 10290, 10297, 10303, 10310, 10317, 10324, 10330, 10337, 10344,
    10351, 10357, 10364, 10371, 10378, 10385, 10391, 10398, 10405, 10412, 10418, 10425, 10432, 10439, 10445, 10452,
    10459, 10465, 10472, 10479, 10486, 10492, 10499, 10506, 10513, 10519, 10526, 10533, 10540, 10546, 10553, 10560,
    10567, 10573, 10580, 10587, 10594, 10600, 10607, 10614, 10620, 10627, 10634, 10641, 10647, 10654, 10661, 10668,
    10674, 10681, 10688, 10694, 10701, 10708, 10715, 10721, 10728, 10735, 10741, 10748, 10755, 10762, 10768, 10775,
    10782, 10788, 10795, 10802, 10809, 10815, 10822, 10829, 10835, 10842, 10849, 10856, 10862, 10869, 10876, 10882,
    10889, 10896, 10902, 10909, 10916, 10923, 10929, 10936, 10943, 10949, 10956, 10963, 10969, 10976, 10983, 10990,
    10996, 11003, 11010, 11016, 11023, 11030, 11036, 11043, 11050, 11056, 11063, 11070, 11076, 11083, 11090, 11096,
    11103, 11110, 11117, 11123, 11130, 11137, 11143, 11150, 11157, 11163, 11170, 11177, 11183, 11190, 11197, 11203,
    11210, 11217, 11223, 11230, 11237, 11243, 11250, 11257, 11263, 11270, 11277, 11283, 11290, 11297, 11303, 11310,
    11317, 11323, 11330, 11336, 11343, 11350, 11356, 11363, 11370, 11376, 11383, 11390, 11396, 11403, 11410, 11416,
    11423, 11430, 11436, 11443, 11449, 11456, 11463, 11469, 11476, 11483, 11489, 11496, 11503, 11509, 11516, 11522,
    11529, 11536, 11542, 11549, 11556, 11562, 11569, 11575, 11582, 11589, 11595, 11602, 11609, 11615, 11622, 11628,
    11635, 11642, 11648, 11655, 11662, 11668, 11675, 11681, 11688, 11695, 11701, 11708, 11714, 11721, 11728, 11734,
    11741, 11747, 11754, 11761, 11767, 11774, 11781, 11787, 11794, 11800, 11807, 11814, 11820, 11827, 11833, 11840,
    11846, 11853, 11860, 11866, 11873, 11879, 11886, 11893, 11899, 11906, 11912, 11919, 11926, 11932, 11939, 11945,
    11952, 11958, 11965, 11972, 11978, 11985, 11991, 11998, 12005, 12011, 12018, 12024, 12031, 12037, 12044, 12051,
    12057, 12064, 12070, 12077, 12083, 12090, 12097, 12103, 12110, 12116, 12123, 12129, 12136, 12142, 12149, 12156,
    12162, 12169, 12175, 12182, 12188, 12195, 12201, 12208, 12215, 12221, 12228, 12234, 12241, 12247, 12254, 12260,
    12267, 12273, 12280, 12287, 12293, 12300, 12306, 12313, 12319, 12326, 12332, 12339, 12345, 12352, 12358, 12365,
    12372, 12378, 12385, 12391, 12398, 12404, 12411, 12417, 12424, 12430, 12437, 12443, 12450, 12456, 12463, 12469,
    12476, 12482, 12489, 12495, 12502, 12509, 12515, 12522, 12528, 12535, 12541, 12548, 12554, 12561, 12567, 12574,
    12580, 12587, 12593, 12600, 12606, 12613, 12619, 12626, 12632, 12639, 12645, 12652, 12658, 12665, 12671, 12678,
    12684, 12691, 12697, 12704, 12710, 12717, 12723, 12730, 12736, 12742, 12749, 12755, 12762, 12768, 12775, 12781,
    12788, 12794, 12801, 12807, 12814, 12820, 12827, 12833, 12840, 12846, 12853, 12859, 12866, 12872, 12878, 12885,
    12891, 12898, 12904, 12911, 12917, 12924, 12930, 12937, 12943, 12950, 12956, 12962, 12969, 12975, 12982, 12988,
    12995, 13001, 13008, 13014, 13021, 13027, 13033, 13040, 13046, 13053, 13059, 13066, 13072, 13079, 13085, 13091,
    13098, 13104, 13111, 13117, 13124, 13130, 13136, 13143, 13149, 13156, 13162, 13169, 13175, 13181, 13188, 13194,
    13201, 13207, 13214, 13220, 13226, 13233, 13239, 13246, 13252, 13259, 13265, 13271, 13278, 13284, 13291, 13297,
    13303, 13310, 13316, 13323, 13329, 13335, 13342, 13348, 13355, 13361, 13367, 13374, 13380, 13387, 13393, 13399,
    13406, 13412, 13419, 13425, 13431, 13438, 13444, 13451, 13457, 13463, 13470, 13476, 13483, 13489, 13495, 13502,
    13508, 13514, 13521, 13527, 13534, 13540, 13546, 13553, 13559, 13565, 13572, 13578, 13585, 13591, 13597, 13604,
    13610, 13616, 13623, 13629, 13636, 13642, 13648, 13655, 13661, 13667, 13674, 13680, 13686, 13693, 13699, 13705,
    13712, 13718, 13725, 13731, 13737, 13744, 13750, 13756, 13763, 13769, 13775, 13782, 13788, 13794, 13801, 13807,
    13813, 13820, 13826, 13832, 13839, 13845, 13851, 13858, 13864, 13870, 13877, 13883, 13889, 13896, 13902, 13908,
    13915, 13921, 13927, 13934, 13940, 13946, 13953, 13959, 13965, 13972, 13978, 13984, 13991, 13997, 14003, 14009,
    14016, 14022, 14028, 14035, 14041, 14047, 14054, 14060, 14066, 14073, 14079, 14085, 14091, 14098, 14104, 14110,
    14117, 14123, 14129, 14136, 14142, 14148, 14154, 14161, 14167, 14173, 14180, 14186, 14192, 14198, 14205, 14211,
    14217, 14224, 14230, 14236, 14242, 14249, 14255, 14261, 14267, 14274, 14280, 14286, 14293, 14299, 14305, 14311,
    14318, 14324, 14330, 14336, 14343, 14349, 14355, 14361, 14368, 14374, 14380, 14386, 14393, 14399, 14405, 14411,
    14418, 14424, 14430, 14436, 14443, 14449, 14455, 14461, 14468, 14474, 14480, 14486, 14493, 14499, 14505, 14511,
    14518, 14524, 14530, 14536, 14543, 14549, 14555, 14561, 14567, 14574, 14580, 14586, 14592, 14599, 14605, 14611,
    14617, 14623, 14630, 14636, 14642, 14648, 14655, 14661, 14667, 14673, 14679, 14686, 14692, 14698, 14704, 14710,
    14717, 14723, 14729, 14735, 14741, 14748, 14754, 14760, 14766, 14772, 14779, 14785, 14791, 14797, 14803, 14810,
    14816, 14822, 14828, 14834, 14841, 14847, 14853, 14859, 14865, 14871, 14878, 14884, 14890, 14896, 14902, 14909,
    14915, 14921, 14927, 14933, 14939, 14946, 14952, 14958, 14964, 14970, 14976, 14983, 14989, 14995, 15001, 15007,
    15013, 15020, 15026, 15032, 15038, 15044, 15050, 15056, 15063, 15069, 15075, 15081, 15087, 15093, 15100, 15106,
    15112, 15118, 15124, 15130, 15136, 15142, 15149, 15155, 15161, 15167, 15173, 15179, 15185, 15192, 15198, 15204,
    15210, 15216, 15222, 15228, 15234, 15241, 15247, 15253, 15259, 15265, 15271, 15277, 15283, 15290, 15296, 15302,
    15308, 15314, 15320, 15326, 15332, 15338, 15344, 15351, 15357, 15363, 15369, 15375, 15381, 15387, 15393, 15399,
    15405, 15412, 15418, 15424, 15430, 15436, 15442, 15448, 15454, 15460, 15466, 15472, 15479, 15485, 15491, 15497,
    15503, 15509, 15515, 15521, 15527, 15533, 15539, 15545, 15551, 15558, 15564, 15570, 15576, 15582, 15588, 15594,
    15600, 15606, 15612, 15618, 15624, 15630, 15636, 15642, 15648, 15655, 15661, 15667, 15673, 15679, 15685, 15691,
    15697, 15703, 15709, 15715, 15721, 15727, 15733, 15739, 15745, 15751, 15757, 15763, 15769, 15775, 15781, 15787,
    15793, 15799, 15806, 15812, 15818, 15824, 15830, 15836, 15842, 15848, 15854, 15860, 15866, 15872, 15878, 15884,
    15890, 15896, 15902, 15908, 15914, 15920, 15926, 15932, 15938, 15944, 15950, 15956, 15962, 15968, 15974, 15980,
    15986, 15992, 15998, 16004, 16010, 16016, 16022, 16028, 16034, 16040, 16046, 16052, 16058, 16064, 16070, 16076,
    16082, 16088, 16094, 16100, 16106, 16112, 16118, 16123, 16129, 16135, 16141, 16147, 16153, 16159, 16165, 16171,
    16177, 16183, 16189, 16195, 16201, 16207, 16213, 16219, 16225, 16231, 16237, 16243, 16249, 16255, 16261, 16267,
    16272, 16278, 16284, 16290, 16296, 16302, 16308, 16314, 16320, 16326, 16332, 16338, 16344, 16350, 16356, 16362,
    16367, 16373, 16379, 16385, 16391, 16397, 16403, 16409, 16415, 16421, 16427, 16433, 16438, 16444, 16450, 16456,
    16462, 16468, 16474, 16480, 16486, 16492, 16498, 16504, 16509, 16515, 16521, 16527, 16533, 16539, 16545, 16551,
    16557, 16562, 16568, 16574, 16580, 16586, 16592, 16598, 16604, 16610, 16615, 16621, 16627, 16633, 16639, 16645,
    16651, 16657, 16663, 16668, 16674, 16680, 16686, 16692, 16698, 16704, 16709, 16715, 16721, 16727, 16733, 16739,
    16745, 16750, 16756, 16762, 16768, 16774, 16780, 16786, 16791, 16797, 16803, 16809, 16815, 16821, 16827, 16832,
    16838, 16844, 16850, 16856, 16862, 16867, 16873, 16879, 16885, 16891, 16897, 16902, 16908, 16914, 16920, 16926,
    16932, 16937, 16943, 16949, 16955, 16961, 16966, 16972, 16978, 16984, 16990, 16996, 17001, 17007, 17013, 17019,
    17025, 17030, 17036, 17042, 17048, 17054, 17059, 17065, 17071, 17077, 17083, 17088, 17094, 17100, 17106, 17112,
    17117, 17123, 17129, 17135, 17140, 17146, 17152, 17158, 17164, 17169, 17175, 17181, 17187, 17193, 17198, 17204,
    17210, 17216, 17221, 17227, 17233, 17239, 17244, 17250, 17256, 17262, 17267, 17273, 17279, 17285, 17290, 17296,
    17302, 17308, 17314, 17319, 17325, 17331, 17336, 17342, 17348, 17354, 17359, 17365, 17371, 17377, 17382, 17388,
    17394, 17400, 17405, 17411, 17417, 17423, 17428, 17434, 17440, 17445, 17451, 17457, 17463, 17468, 17474, 17480,
    17485, 17491, 17497, 17503, 17508, 17514, 17520, 17525, 17531, 17537, 17543, 17548, 17554, 17560, 17565, 17571,
    17577, 17582, 17588, 17594, 17600, 17605, 17611, 17617, 17622, 17628, 17634, 17639, 17645, 17651, 17656, 17662,
    17668, 17673, 17679, 17685, 17691, 17696, 17702, 17708, 17713, 17719, 17725, 17730, 17736, 17742, 17747, 17753,
    17759, 17764, 17770, 17775, 17781, 17787, 17792, 17798, 17804, 17809, 17815, 17821, 17826, 17832, 17838, 17843,
    17849, 17855, 17860, 17866, 17871, 17877, 17883, 17888, 17894, 17900, 17905, 17911, 17917, 17922, 17928, 17933,
    17939, 17945, 17950, 17956, 17962, 17967, 17973, 17978, 17984, 17990, 17995, 18001, 18006, 18012, 18018, 18023,
    18029, 18034, 18040, 18046, 18051, 18057, 18062, 18068, 18074, 18079, 18085, 18090, 18096, 18102, 18107, 18113,
    18118, 18124, 18130, 18135, 18141, 18146, 18152, 18157, 18163, 18169, 18174, 18180, 18185, 18191, 18196, 18202,
    18208, 18213, 18219, 18224, 18230, 18235, 18241, 18247, 18252, 18258, 18263, 18269, 18274, 18280, 18285, 18291,
    18297, 18302, 18308, 18313, 18319, 18324, 18330, 18335, 18341, 18346, 18352, 18357, 18363, 18369, 18374, 18380,
    18385, 18391, 18396, 18402, 18407, 18413, 18418, 18424, 18429, 18435, 18440, 18446, 18451, 18457, 18462, 18468,
    18473, 18479, 18484, 18490, 18495, 18501, 18506, 18512, 18517, 18523, 18528, 18534, 18539, 18545, 18550, 18556,
    18561, 18567, 18572, 18578, 18583, 18589, 18594, 18600, 18605, 18611, 18616, 18622, 18627, 18633, 18638, 18644,
    18649, 18655, 18660, 18665, 18671, 18676, 18682, 18687, 18693, 18698, 18704, 18709, 18715, 18720, 18726, 18731,
    18736, 18742, 18747, 18753, 18758, 18764, 18769, 18775, 18780, 18785, 18791, 18796, 18802, 18807, 18813, 18818,
    18823, 18829, 18834, 18840, 18845, 18851, 18856, 18861, 18867, 18872, 18878, 18883, 18889, 18894, 18899, 18905,
    18910, 18916, 18921, 18926, 18932, 18937, 18943, 18948, 18953, 18959, 18964, 18970, 18975, 18980, 18986, 18991,
    18997, 19002, 19007, 19013, 19018, 19024, 19029, 19034, 19040, 19045, 19050, 19056, 19061, 19067, 19072, 19077,
    19083, 19088, 19093, 19099, 19104, 19110, 19115, 19120, 19126, 19131, 19136, 19142, 19147, 19152, 19158, 19163,
    19169, 19174, 19179, 19185, 19190, 19195, 19201, 19206, 19211, 19217, 19222, 19227, 19233, 19238, 19243, 19249,
    19254, 19259, 19265, 19270, 19275, 19281, 19286, 19291, 19297, 19302, 19307, 19313, 19318, 19323, 19328, 19334,
    19339, 19344, 19350, 19355, 19360, 19366, 19371, 19376, 19382, 19387, 19392, 19397, 19403, 19408, 19413, 19419,
    19424, 19429, 19434, 19440, 19445, 19450, 19456, 19461, 19466, 19471, 19477, 19482, 19487, 19493, 19498, 19503,
    19508, 19514, 19519, 19524, 19529, 19535, 19540, 19545, 19551, 19556, 19561, 19566, 19572, 19577, 19582, 19587,
    19593, 19598, 19603, 19608, 19614, 19619, 19624, 19629, 19635, 19640, 19645, 19650, 19655, 19661, 19666, 19671,
    19676, 19682, 19687, 19692, 19697, 19703, 19708, 19713, 19718, 19723, 19729, 19734, 19739, 19744, 19749, 19755,
    19760, 19765, 19770, 19776, 19781, 19786, 19791, 19796, 19802, 19807, 19812, 19817, 19822, 19827, 19833, 19838,
    19843, 19848, 19853, 19859, 19864, 19869, 19874, 19879, 19885, 19890, 19895, 19900, 19905, 19910, 19916, 19921,
    19926, 19931, 19936, 19941, 19947, 19952, 19957, 19962, 19967, 19972, 19978, 19983, 19988, 19993, 19998, 20003,
    20008, 20014, 20019, 20024, 20029, 20034, 20039, 20044, 20050, 20055, 20060, 20065, 20070, 20075, 20080, 20085,
    20091, 20096, 20101, 20106, 20111, 20116, 20121, 20126, 20132, 20137, 20142, 20147, 20152, 20157, 20162, 20167,
    20172, 20178, 20183, 20188, 20193, 20198, 20203, 20208, 20213, 20218, 20223, 20228, 20234, 20239, 20244, 20249,
    20254, 20259, 20264, 20269, 20274, 20279, 20284, 20289, 20295, 20300, 20305, 20310, 20315, 20320, 20325, 20330,
    20335, 20340, 20345, 20350, 20355, 20360, 20365, 20370, 20376, 20381, 20386, 20391, 20396, 20401, 20406, 20411,
    20416, 20421, 20426, 20431, 20436, 20441, 20446, 20451, 20456, 20461, 20466, 20471, 20476, 20481, 20486, 20491,
    20496, 20501, 20506, 20511, 20516, 20521, 20526, 20531, 20536, 20541, 20546, 20551, 20556, 20561, 20566, 20571,
    20576, 20581, 20586, 20591, 20596, 20601, 20606, 20611, 20616, 20621, 20626, 20631, 20636, 20641, 20646, 20651,
    20656, 20661, 20666, 20671, 20676, 20681, 20686, 20691, 20696, 20701, 20706, 20711, 20716, 20721, 20726, 20731,
    20736, 20741, 20746, 20751, 20756, 20760, 20765, 20770, 20775, 20780, 20785, 20790, 20795, 20800, 20805, 20810,
    20815, 20820, 20825, 20830, 20835, 20839, 20844, 20849, 20854, 20859, 20864, 20869, 20874, 20879, 20884, 20889,
    20894, 20898, 20903, 20908, 20913, 20918, 20923, 20928, 20933, 20938, 20943, 20948, 20952, 20957, 20962, 20967,
    20972, 20977, 20982, 20987, 20992, 20996, 21001, 21006, 21011, 21016, 21021, 21026, 21031, 21035, 21040, 21045,
    21050, 21055, 21060, 21065, 21069, 21074, 21079, 21084, 21089, 21094, 21099, 21103, 21108, 21113, 21118, 21123,
    21128, 21133, 21137, 21142, 21147, 21152, 21157, 21162, 21166, 21171, 21176, 21181, 21186, 21191, 21195, 21200,
    21205, 21210, 21215, 21220, 21224, 21229, 21234, 21239, 21244, 21248, 21253, 21258, 21263, 21268, 21272, 21277,
    21282, 21287, 21292, 21296, 21301, 21306, 21311, 21316, 21320, 21325, 21330, 21335, 21340, 21344, 21349, 21354,
    21359, 21363, 21368, 21373, 21378, 21383, 21387, 21392, 21397, 21402, 21406, 21411, 21416, 21421, 21425, 21430,
    21435, 21440, 21444, 21449, 21454, 21459, 21463, 21468, 21473, 21478, 21482, 21487, 21492, 21497, 21501, 21506,
    21511, 21516, 21520, 21525, 21530, 21534, 21539, 21544, 21549, 21553, 21558, 21563, 21568, 21572, 21577, 21582,
    21586, 21591, 21596, 21601, 21605, 21610, 21615, 21619, 21624, 21629, 21633, 21638, 21643, 21648, 21652, 21657,
    21662, 21666, 21671, 21676, 21680, 21685, 21690, 21694, 21699, 21704, 21708, 21713, 21718, 21722, 21727, 21732,
    21736, 21741, 21746, 21750, 21755, 21760, 21764, 21769, 21774, 21778, 21783, 21788, 21792, 21797, 21802, 21806,
    21811, 21816, 21820, 21825, 21829, 21834, 21839, 21843, 21848, 21853, 21857, 21862, 21866, 21871, 21876, 21880,
    21885, 21890, 21894, 21899, 21903, 21908, 21913, 21917, 21922, 21926, 21931, 21936, 21940, 21945, 21950, 21954,
    21959, 21963, 21968, 21972, 21977, 21982, 21986, 21991, 21995, 22000, 22005, 22009, 22014, 22018, 22023, 22028,
    22032, 22037, 22041, 22046, 22050, 22055, 22059, 22064, 22069, 22073, 22078, 22082, 22087, 22091, 22096, 22101,
    22105, 22110, 22114, 22119, 22123, 22128, 22132, 22137, 22141, 22146, 22151, 22155, 22160, 22164, 22169, 22173,
    22178, 22182, 22187, 22191, 22196, 22200, 22205, 22209, 22214, 22218, 22223, 22227, 22232, 22236, 22241, 22245,
    22250, 22254, 22259, 22263, 22268, 22272, 22277, 22281, 22286, 22290, 22295, 22299, 22304, 22308, 22313, 22317,
    22322, 22326, 22331, 22335, 22340, 22344, 22349, 22353, 22358, 22362, 22367, 22371, 22376, 22380, 22384, 22389,
    22393, 22398, 22402, 22407, 22411, 22416, 22420, 22425, 22429, 22433, 22438, 22442, 22447, 22451, 22456, 22460,
    22464, 22469, 22473, 22478, 22482, 22487, 22491, 22495, 22500, 22504, 22509, 22513, 22518, 22522, 22526, 22531,
    22535, 22540, 22544, 22548, 22553, 22557, 22562, 22566, 22570, 22575, 22579, 22584, 22588, 22592, 22597, 22601,
    22606, 22610, 22614, 22619, 22623, 22628, 22632, 22636, 22641, 22645, 22649, 22654, 22658, 22663, 22667, 22671,
    22676, 22680, 22684, 22689, 22693, 22697, 22702, 22706, 22710, 22715, 22719, 22724, 22728, 22732, 22737, 22741,
    22745, 22750, 22754, 22758, 22763, 22767, 22771, 22776, 22780, 22784, 22789, 22793, 22797, 22802, 22806, 22810,
    22814, 22819, 22823, 22827, 22832, 22836, 22840, 22845, 22849, 22853, 22858, 22862, 22866, 22870, 22875, 22879,
    22883, 22888, 22892, 22896, 22900, 22905, 22909, 22913, 22918, 22922, 22926, 22930, 22935, 22939, 22943, 22948,
    22952, 22956, 22960, 22965, 22969, 22973, 22977, 22982, 22986, 22990, 22994, 22999, 23003, 23007, 23011, 23016,
    23020, 23024, 23028, 23033, 23037, 23041, 23045, 23050, 23054, 23058, 23062, 23067, 23071, 23075, 23079, 23083,
    23088, 23092, 23096, 23100, 23104, 23109, 23113, 23117, 23121, 23126, 23130, 23134, 23138, 23142, 23147, 23151,
    23155, 23159, 23163, 23168, 23172, 23176, 23180, 23184, 23188, 23193, 23197, 23201, 23205, 23209, 23214, 23218,
    23222, 23226, 23230, 23234, 23239, 23243, 23247, 23251, 23255, 23259, 23264, 23268, 23272, 23276, 23280, 23284,
    23288, 23293, 23297, 23301, 23305, 23309, 23313, 23317, 23322, 23326, 23330, 23334, 23338, 23342, 23346, 23350,
    23355, 23359, 23363, 23367, 23371, 23375, 23379, 23383, 23388, 23392, 23396, 23400, 23404, 23408, 23412, 23416,
    23420, 23424, 23429, 23433, 23437, 23441, 23445, 23449, 23453, 23457, 23461, 23465, 23469, 23473, 23478, 23482,
    23486, 23490, 23494, 23498, 23502, 23506, 23510, 23514, 23518, 23522, 23526, 23530, 23534, 23539, 23543, 23547,
    23551, 23555, 23559, 23563, 23567, 23571, 23575, 23579, 23583, 23587, 23591, 23595, 23599, 23603, 23607, 23611,
    23615, 23619, 23623, 23627, 23631, 23635, 23639, 23643, 23647, 23651, 23655, 23659, 23663, 23667, 23671, 23675,
    23679, 23683, 23687, 23691, 23695, 23699, 23703, 23707, 23711, 23715, 23719, 23723, 23727, 23731, 23735, 23739,
    23743, 23747, 23751, 23755, 23759, 23763, 23767, 23771, 23775, 23779, 23783, 23787, 23791, 23795, 23799, 23803,
    23807, 23811, 23815, 23818, 23822, 23826, 23830, 23834, 23838, 23842, 23846, 23850, 23854, 23858, 23862, 23866,
    23870, 23874, 23877, 23881, 23885, 23889, 23893, 23897, 23901, 23905, 23909, 23913, 23917, 23921, 23924, 23928,
    23932, 23936, 23940, 23944, 23948, 23952, 23956, 23959, 23963, 23967, 23971, 23975, 23979, 23983, 23987, 23991,
    23994, 23998, 24002, 24006, 24010, 24014, 24018, 24022, 24025, 24029, 24033, 24037, 24041, 24045, 24049, 24052,
    24056, 24060, 24064, 24068, 24072, 24075, 24079, 24083, 24087, 24091, 24095, 24098, 24102, 24106, 24110, 24114,
    24118, 24121, 24125, 24129, 24133, 24137, 24141, 24144, 24148, 24152, 24156, 24160, 24163, 24167, 24171, 24175,
    24179, 24182, 24186, 24190, 24194, 24198, 24201, 24205, 24209, 24213, 24217, 24220, 24224, 24228, 24232, 24235,
    24239, 24243, 24247, 24250, 24254, 24258, 24262, 24266, 24269, 24273, 24277, 24281, 24284, 24288, 24292, 24296,
    24299, 24303, 24307, 24311, 24314, 24318, 24322, 24326, 24329, 24333, 24337, 24340, 24344, 24348, 24352, 24355,
    24359, 24363, 24367, 24370, 24374, 24378, 24381, 24385, 24389, 24393, 24396, 24400, 24404, 24407, 24411, 24415,
    24418, 24422, 24426, 24430, 24433, 24437, 24441, 24444, 24448, 24452, 24455, 24459, 24463, 24466, 24470, 24474,
    24477, 24481, 24485, 24488, 24492, 24496, 24499, 24503, 24507, 24510, 24514, 24518, 24521, 24525, 24529, 24532,
    24536, 24540, 24543, 24547, 24551, 24554, 24558, 24561, 24565, 24569, 24572, 24576
};