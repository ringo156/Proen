
// 迷惑メール対策 (メールアドレスを収集されないようにするため) に
// 分解して記述してあります

// スパム防止の為に、@ を省く
// hoge@gmail.com -> hoge+g
// 最後の一文字はドメインを表す
// ドメインは別途定義する
var MAIL = {
	kutpg    : "nullptr.kut+g",
	cast     : "cast.desktop+g",
	robotclub: "robotclub_kut+y",
	denso    : "densou_force+h",
	kutar    : "kutar2011+g",
	tansya   : "150314x+k",
    kakashi  : "dennoukakasi.kut+g",
    webworks : "kut.web.works+g",
    kuttech : "kut.techclub+g"
};

// メールアドレスのドメインを定義する
var DOMAIN = {
	g: "gmail.com",
	y: "yahoo.co.jp",
	h: "hotmail.com",
	k: "ugs.kochi-tech.ac.jp"
};

function mail_club_convert(club){
	return club.gsub("_", "").gsub("-", "").gsub(".", "").toLowerCase();
}

function mail_convert(club){
	var club   = mail_club_convert(club);
	var local  = MAIL[club].slice(0, MAIL[club].lastIndexOf("+"));
	var domain = DOMAIN[MAIL[club].slice(MAIL[club].lastIndexOf("+") + 1)];
	
	return local + "@" + domain;
}

function mail_text(club){
	document.write(mail_convert(club));
}

function mail_qrcode(club, size){
	var size = size | 100;
	
	document.write(
		'<img src="https://chart.googleapis.com/chart?chs=' + size + 'x' + size +
		'&cht=qr&chl=' + encodeURIComponent(mail_convert(club)) + '" alt="' + mail_convert(club) + '" ' +
		'width="' + size + '" height="' + size + '" />'
	);
}
