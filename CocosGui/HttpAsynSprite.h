//
//  HttpAsynSprite.h
//  GUI
//  shengxiaainj@gmail.com
//  Created by LeeWei on 15/10/22.
//
//

#ifndef HttpAsynSprite_h
#define HttpAsynSprite_h

#include "cocos2d.h"
#include "../macros.h"

USING_NS_CC;

NS_CC_GUI_BEGIN

struct OutputStruct
{
    OutputStruct()
    : buffer(nullptr)
    , size(0)
    {
    };
    char * buffer;
    size_t size;
};

class HttpAsynSprite : public Sprite
{
public:
	static HttpAsynSprite* create(const std::string& defauleFileName);
	static HttpAsynSprite* create(const std::string& defauleFileName, const std::string& url);
	static HttpAsynSprite* createWithURL(const std::string& url);

	bool initWithUrl(const std::string& url);
	bool initWithUrl(const std::string& defauleFileName, const std::string& url);

	CC_SYNTHESIZE_READONLY(std::string, m_url, Url);

	void setUrl(const std::string& url);
	void setSavePath(const std::string& filePath);
	std::string getSavePath() { return m_sSavePath; }
	void startDownload();
private:
	std::string m_sSavePath;
	OutputStruct m_urlOutput;
	bool m_bDownloading;

	HttpAsynSprite();
	~HttpAsynSprite();

	std::string stringKeyFilter(const std::string& srcString);
protected:
	bool initWithFile(const std::string& filename);
	void download(const std::string &url);
	bool receiveImage(const std::string& url);
	void update(float dt);
};

NS_CC_GUI_END

#endif /* HttpAsynSprite_h */