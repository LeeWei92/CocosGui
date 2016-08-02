//
//  HttpAsynSprite.cpp
//  GUI
//  shengxiaainj@gmail.com
//  Created by LeeWei on 15/10/22.
//
//

#include "HttpAsynSprite.h"
#include <fstream>
#include <curl/curl.h>

NS_CC_GUI_BEGIN

size_t writeData(void *ptr, size_t size, size_t nmemb, void *data)
{
	size_t realsize = size * nmemb;

	struct OutputStruct *mem = (struct OutputStruct *) data;

	mem->buffer = (char*)realloc(mem->buffer, mem->size + realsize + 1);

	if (mem->buffer)
	{
		memcpy(&(mem->buffer[mem->size]), ptr, realsize);
		mem->size += realsize;
		mem->buffer[mem->size] = 0;
	}
	return realsize;
}

int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
	static int percent = 0;
	int tmp = 0;
	if (totalToDownload > 0)
	{
		tmp = (int)(nowDownloaded / totalToDownload * 100);
	}

	/*
	if ((static_cast<HttpAsynSprite*>(ptr))->_cancelDownload)
	{
	curl_easy_pause((static_cast<HttpAsynSprite*>(ptr))->_curl, CURLPAUSE_RECV);
	curl_easy_cleanup((static_cast<HttpAsynSprite*>(ptr))->_curl);
	fclose((static_cast<HttpAsynSprite*>(ptr))->_fp);
	return 0;
	}
	*/

	if (percent != tmp)
	{
		percent = tmp;
		/*
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
		auto manager = static_cast<HttpAsynSprite*>(ptr);
		if (manager->_delegate)
		manager->_delegate->onProgress(percent);
		});
		*/
		CCLOG("downloading... %d%%", percent);
	}

	return 0;
}

HttpAsynSprite::HttpAsynSprite()
: m_bDownloading(false)
{
	m_sSavePath = FileUtils::getInstance()->getWritablePath();
}

HttpAsynSprite::~HttpAsynSprite()
{
	unschedule(CC_SCHEDULE_SELECTOR(HttpAsynSprite::update));
}

HttpAsynSprite* HttpAsynSprite::create(const std::string &defauleFileName)
{
	auto sprite = new (std::nothrow) HttpAsynSprite();
	if (sprite && sprite->initWithFile(defauleFileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

HttpAsynSprite* HttpAsynSprite::create(const std::string &defauleFileName,
	const std::string &url)
{
	auto sprite = new (std::nothrow) HttpAsynSprite();
	if (sprite && sprite->initWithUrl(defauleFileName, url))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

HttpAsynSprite* HttpAsynSprite::createWithURL(const std::string& url)
{
	auto sprite = new (std::nothrow) HttpAsynSprite();
	if (sprite && sprite->initWithUrl(url))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool HttpAsynSprite::initWithFile(const std::string& filename)
{
	if (filename.size() <= 0)
	{
		return false;
	}
	auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(filename);
	if (frame)
	{
		return Sprite::initWithSpriteFrameName(filename);
	}
	else
	{
		return Sprite::initWithFile(filename);
	}
	return true;
}

bool HttpAsynSprite::initWithUrl(const std::string &defauleFileName, const std::string &url)
{
	if (!initWithFile(defauleFileName))
	{
		return false;
	}

	m_url = url;

	startDownload();

	return true;
}

bool HttpAsynSprite::initWithUrl(const std::string &url)
{
	if (!Sprite::init())
	{
		return false;
	}

	m_url = url;

	startDownload();

	return true;
}

void HttpAsynSprite::setUrl(const std::string &url)
{
	m_url = url;
}

void HttpAsynSprite::setSavePath(const std::string& filePath)
{
	if (!FileUtils::getInstance()->isDirectoryExist(filePath))
	{
		FileUtils::getInstance()->createDirectory(filePath);
	}
	m_sSavePath = filePath;
}

void HttpAsynSprite::startDownload()
{
	if (0 >= m_url.length()) return;

	size_t index = m_url.find_last_of('/');
	std::string fileName;
	if (0 < index)
	{
		fileName = m_url.substr(index + 1, m_url.length());
	}

	std::string filePath = m_sSavePath + fileName;
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(filePath);
	if (texture)
	{
		setTexture(filePath);
	}
	else
	{
		// http start downloading
		log("startDownload fileName %s", fileName.c_str());
		m_url = stringKeyFilter(m_url);

		if (m_bDownloading) return;

		std::thread(&HttpAsynSprite::download, this, m_url).detach();

		if (!m_bDownloading)
		{
			schedule(CC_SCHEDULE_SELECTOR(HttpAsynSprite::update));
			m_bDownloading = true;
		}
	}
}

void HttpAsynSprite::download(const std::string &url)
{
	do
	{
		if (!receiveImage(url)) break;
	} while (0);
}

bool HttpAsynSprite::receiveImage(const std::string& url)
{
	CURL *curl;
	CURLcode res;

	struct OutputStruct output;
	output.buffer = NULL;
	output.size = 0;

	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&output);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		/*
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, this);
		*/
		curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 1);
		curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 5);

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);
		if (res == 0)
		{
			m_urlOutput = output;
			log("*SUCCEED* Get image from url %s", url.c_str());
			return true;
		}
		log("*FAILED* Get image from url %s with Error: %i", url.c_str(), res);
	}

	return false;
}

void HttpAsynSprite::update(float dt)
{
	if (m_urlOutput.buffer && m_urlOutput.size > 0)
	{
		size_t index = m_url.find_last_of('/');
		std::string fileName;
		if (0 < index)
		{
			fileName = m_url.substr(index + 1, m_url.length());
		}
		std::string filePath = m_sSavePath + fileName;
		if (FileUtils::getInstance()->isFileExist(filePath))
			FileUtils::getInstance()->removeFile(filePath);
		FILE *pFile = fopen(filePath.c_str(), "rb");
		if (!pFile)
		{
			std::ofstream fout(filePath.c_str(), std::ios::binary | std::ios::app);
			for (unsigned int i = 0; i < m_urlOutput.size; ++i)
			{
				fout.write(&(m_urlOutput.buffer)[i], sizeof(char));
			}
			fout.close();
			log("%s file download complete!", fileName.c_str());
			setTexture(filePath);
		}
		if (pFile)
		{
			fclose(pFile);
		}

		m_bDownloading = false;
		unschedule(CC_SCHEDULE_SELECTOR(HttpAsynSprite::update));
	}
}

std::string HttpAsynSprite::stringKeyFilter(const std::string& srcString)
{
	std::string result = srcString;
	for (auto it = result.begin(); it != result.end(); ++it)
	{
		const char* key = "/";
		if (0 == strcmp(&(*it), key))
		{
			result.erase(it);
		}
	}

	return result;
}

NS_CC_GUI_END