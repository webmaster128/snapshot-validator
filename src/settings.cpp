#include "settings.h"

#include "utils.h"

namespace {
const auto BPL = 100000000; // Beddows per Lisk
}

Settings::Settings(Network network)
{
    switch (network) {
    case Network::Mainnet:
        genesisBlock = 13658550407518916215ul;
        negativeBalanceAddress = 6566229458323231555ul;
        rewardOffset = 1451520;
        rewardDistance = 3000000;
        blockVersions[1] = 0;
        blockVersions[6901027 + 1] = 1;
        // https://github.com/LiskHQ/lisk/blob/0.9.16/genesisBlock.json
        exceptions.transactionsSignedBy[genesisBlock] = hex2Bytes("d121d3abf5425fdc0f161d9ddb32f89b7750b4bdb0bff7d18b191d4b4bafa6d4");
        exceptions.freeTransactionsBlockId = genesisBlock;
        exceptions.invalidTransactionSignature = {
            5676385569187187158ul, // 868797
            5384302058030309746ul, // 869890
            9352922026980330230ul, // 925165
        };
        exceptions.transactionsContainingInvalidRecipientAddress = {
            // out of uint64 range
            // select * from (select "blockId", id as "transactionId", "recipientId", CAST(left("recipientId", -1) AS numeric) AS address_number FROM trs ORDER BY "rowId") as converted_table WHERE address_number > 18446744073709551615
            4808146167169807212ul,
            8662249085950135942ul,
            3512842658681414759ul,
            16490300774781935982ul,
            952064562393713903ul,
            11106640899982774712ul,
            14173229905148528539ul,
            10002297844266128985ul,
            15618125819389758504ul,
            14272734810070193586ul,
            2183159486043742102ul,
            939135978005147115ul,
            16460700234760525809ul,
            2756544599198077295ul,
            7926138601823684757ul,
            12636129598615060450ul,
            3402607277804364801ul,
            1040149454925709310ul,
            12358300408347126016ul,
            16085334466827014444ul,
            3054377757145973904ul,
            15607047602866731136ul,
            13087133649353348448ul,
            101792401786565558ul,
            12661581926779974683ul,
            13602311678584165723ul,
            1431099360354287373ul,
            15415858778872512497ul,
            6428153862282818223ul,
            11259766765373809175ul,
            9458819938766898698ul,
            15777929444386211489ul,
            2398720519848481371ul,
            3920240924573375078ul,
            836098833121929369ul,
            12637095980567585324ul,
            516933903766323623ul,
            7874437747681346480ul,
            6762151515473413512ul,
            4894138504028948034ul,
            2865853236604099445ul,
            4740161550333445515ul,
            1459096958324133814ul,
            17599831349987747578ul,
            16887688753571112156ul,
            11215230856097582828ul,
            14140283825150563894ul,
            10531191392368229062ul,

            // leading 0
            // select "id" from trs where left("recipientId", 1) = '0' and "recipientId" != '0L' ORDER BY "rowId"
            12466861689592168447ul,
            14828166242732404834ul,
            7881241259922057838ul,
            15335820464138247723ul,
            10790881043084628952ul,
            8222472670929877652ul,
            321301056789688144ul,
            17221198586575098918ul,
            8985252027779519944ul,
            16386368722107610263ul,
            8809409684590273069ul,
            6407377315551276342ul,
            16457701476824471729ul,
        };
        exceptions.invalidAddresses = {
            // out of uint64 range
            // select * from (select "blockId", id as "transactionId", "recipientId", CAST(left("recipientId", -1) AS numeric) AS address_number FROM trs ORDER BY "rowId") as converted_table WHERE address_number > 18446744073709551615
            "88888888888888888888L",
            "111291927890909688453L",
            "45552822168800676881L",
            "102578089172695223748L",
            "158417393714384967784L",
            "66767893317355082007L",
            "79378290046298522419L",
            "82995860846885414674L",
            "146740799137033984447L",
            "27431371898457477021L",
            "122761091781844220769L",
            "115512977373004367295L",
            "167592467447211905329L",
            "24914870879919111310L",
            "658085858590467165179L",
            "65674106542041665570L",
            "136650599037076114683L",
            "62102519165446322358L",
            "36002663650389933742L",
            "61294630583900822405L",
            "23339304444138683202L",
            "163791171589406644447L",
            "114118779455743828526L",
            "60908836751653554044L",
            "163791171589406644447L",
            "71066000334892806863L",
            "26123009066683693893L",
            "113843230015664032748L",
            "19818282826136809774L",
            "123396601130007835060L",
            "83930832964751654400L",
            "90572553121175565083L",
            "115247857947648814503L",
            "28266953424177981656L",
            "163709399912253300453L",
            "101738091419252525441L",
            "527528572855252525252L",
            "163994490445041326334L",
            "112878737683770362496L",
            "116244144370251521378L",
            "48172455550954660929L",
            "91150325309845111438L",
            "52227326644342588233L",
            "172751402580997820397L",
            "134870701874274944551L",
            "33882703892445210381L",
            "161348288408228933736L",
            "57178850733351210759L",

            // leading 0 addresses
            // select "recipientId" from trs where left("recipientId", 1) = '0' and "recipientId" != '0L' ORDER BY "rowId"
            "07280969963593626387L",
            "03708552248146906277L",
            "000000133700000L",
            "00000000000000000000L",
            "00000000000000L",
            "0644846081578550031L",
            "00702085012798728072L",
            "0670887445780012928L",
            "0461359979913215833L",
            "0918279345171678541L",
            "017643479995130895701L",
            "0605255118852574422L",
            "011359068057580646659L",
        };
        exceptions.payloadHashMismatch = {
            // transactions that cannot be serialized (recipient address exceeding uint64 range)
            // select * from (select "blockId", id as "transactionId", "recipientId", CAST(left("recipientId", -1) AS numeric) AS address_number FROM trs ORDER BY "rowId") as converted_table WHERE address_number > 18446744073709551615
            2324835914503631349ul,
            4697902217642625397ul,
            8978496973888459347ul,
            17067558332600857843ul,
            14396714627441765038ul,
            8895545467181971142ul,
            15251460520767932788ul,
            12391217984042066071ul,
            10918179709929665932ul,
            17503372102374390106ul,
            16299254151901317325ul,
            6240247326030267420ul,
            1409974304137954876ul,
            17925428688616608543ul,
            12870524761881752159ul,
            11124337960677375012ul,
            16135385503955443116ul,
            15524356244874288151ul,
            11037619976427158801ul,
            13305478121937941034ul,
            4289996684427206792ul,
            15045649278111090399ul,
            3868747159580661846ul,
            2837020936480112032ul,
            9912064704912456481ul,
            5318429078106164009ul,
            10965969864719825566ul,
            1728091609855530401ul,
            11380599274852897909ul,
            15905214266106258760ul,
            13088035161322176442ul,
            1332548098504543132ul,
            4560552380137081369ul,
            949419498778867546ul,
            6050909935474896393ul,
            17322779671331134338ul,
            2819656608653946841ul,
            8642896734392050992ul,
            2819670263521618333ul,
            110838895608085721ul,
            5973733860238868463ul,
            10777997293247427064ul,
            7571736313605719182ul,
            676747092111944213ul,
            2008892961948134898ul,
            7937493560646705550ul,
            4715062299837042610ul,
            15709445422377334274ul,

            // transactions that cannot be serialized (leading 0s in recipient address)
            // select "blockId" from trs where left("recipientId", 1) = '0' and "recipientId" != '0L' ORDER BY "rowId"
            14432800562034667268ul,
            17442740313087375072ul,
            12832958688013564378ul,
            15299120174069709992ul,
            15124210853377088910ul,
            10289453077379053142ul,
            4375405149970605729ul,
            2525432370937483187ul,
            15141196187573062254ul,
            15475784852310402086ul,
            6543807808453317489ul,
            16894219033681021958ul,
            3076891028787755764ul,
        };
        exceptions.rewardsFactor[27040] = 2;
        exceptions.feesFactor[27040] = 2;
        exceptions.feesBonus[27040] = 10000000;
        break;
    case Network::Testnet:
        genesisBlock = 7807109686729042739ul;
        negativeBalanceAddress = 16009998050678037905ul;
        rewardOffset = 2160;
        rewardDistance = 3000000;
        blockVersions[1] = 0;
        blockVersions[5932033 + 1] = 1;
        // https://github.com/LiskHQ/lisk/blob/v1.0.0-rc.1/genesis_block.json#L12
        exceptions.transactionsSignedBy[genesisBlock] = hex2Bytes("73ec4adbd8f99f0d46794aeda3c3d86b245bd9d27be2b282cdd38ad21988556b");
        exceptions.freeTransactionsBlockId = genesisBlock;
        exceptions.invalidTransactionSignature = {
            3274071402587084244ul,  // 595491
            10403141873189588012ul, // 624550
            16896494584440078079ul, // 631670
        };
        exceptions.inertTransactions = {
            16394286522174687330ul, // 1318685
            12298100805070303137ul, // 3057955
        };
        exceptions.transactionsContainingInvalidRecipientAddress = {
            // out of uint64 range
            // select * from (select "blockId", id as "transactionId", "recipientId", CAST(left("recipientId", -1) AS numeric) AS address_number FROM trs ORDER BY "rowId") as converted_table WHERE address_number > 18446744073709551615
            393955899193580559ul,
            2595217996098726177ul,
            2851909953078287800ul,

            // leading 0
            // select "id" from trs where left("recipientId", 1) = '0' and "recipientId" != '0L' ORDER BY "rowId"
            12710869213547423905ul,
            4595252596856199985ul,
            4962453608347426857ul,
            14029161570134180080ul,
            11850546615651855419ul,
        };
        exceptions.invalidAddresses = {
            // out of uint64 range
            // select * from (select "blockId", id as "transactionId", "recipientId", CAST(left("recipientId", -1) AS numeric) AS address_number FROM trs ORDER BY "rowId") as converted_table WHERE address_number > 18446744073709551615
            "19961131544040416558L",
            "20906309950204158498L",
            "221360928884514619392L",

            // leading 0 addresses
            // select "recipientId" from trs where left("recipientId", 1) = '0' and "recipientId" != '0L' ORDER BY "rowId"
            "000123L",
            "06076671634347365051L",
            "03333333333333333333L",
            "0123L",
        };
        exceptions.payloadHashMismatch = {
            // transactions that cannot be serialized (recipient address exceeding uint64 range)
            // select * from (select "blockId", id as "transactionId", "recipientId", CAST(left("recipientId", -1) AS numeric) AS address_number FROM trs ORDER BY "rowId") as converted_table WHERE address_number > 18446744073709551615
            2748170801018258119ul,
            2405379395106279240ul,
            15690289703032532260ul,

            // transactions that cannot be serialized (leading 0s in recipient address)
            // select "blockId" from trs where left("recipientId", 1) = '0' and "recipientId" != '0L' ORDER BY "rowId"
            7394590590101043180ul,
            7230686330098219506ul,
            9005812879250298485ul,
            6121878732024067644ul,
            10148279880657592281ul,
        };
        exceptions.balanceAdjustments[15449731671927352923ul] = -1 * BPL; // Burned 1 LSK as `amount` in a delegate vote
        exceptions.blockRewards[2161] = 2 * BPL;
        exceptions.blockRewards[2162] = 1 * BPL;
        break;
    }
}
