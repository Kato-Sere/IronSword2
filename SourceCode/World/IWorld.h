#ifndef IWORLD_H_
#define IWORLD_H_

#include <string>
#include <vector>
#include <gslib.h>

// 前方宣言
class Actor;
class ActorManager;
class Field;
class PlayerUI;

// ワールド抽象インターフェース 授業教材使用
class IWorld {
public:
    // 仮想デストラクタ
    virtual ~IWorld() = default;

    // カメラを追加
    virtual void add_camera(Actor* camera) = 0;
    // ライトの追加
    virtual void add_light(Actor* light) = 0;
    // フィールドの追加
    virtual void add_field(Field* field) = 0;
    //  UIの追加
    virtual void add_ui(Actor* ui) = 0;
    // アクターを追加
    virtual void add_actor(Actor* actor) = 0;
    // ウェイポイント配列の要素数設定
    virtual void set_size_waypoint_vector() = 0;
    // ウェイポイントを配列に追加
    virtual void add_waypoint(int x, int y, Actor* point) = 0;

    // アクターマネージャーの取得
    virtual ActorManager& actor_manager() = 0;
    // アクターの検索
    virtual Actor* find_actor(const std::string& name) const = 0;
    // 指定タグを持つアクターの検索
    virtual std::vector<Actor*> find_actor_with_tag(const GSuint& tag) const = 0;
    // アクター数を返す
    virtual int count_actor() const = 0;
    // 指定したタグを持つアクター数を返す
    virtual int count_actor_with_tag(const GSuint& tag) const = 0;
    // メッセージの送信
    virtual void send_message(const std::string& message, void* param = nullptr) = 0;

    // ゲーム再生速度の取得
    virtual const float game_speed() const = 0;
    // ゲーム再生速度の変更
    virtual void change_speed(float value) = 0;

    // フィールドの取得
    virtual Field* field() = 0;
    // カメラの取得
    virtual Actor* camera() = 0;
    // ライトの取得
    virtual Actor* light() = 0;
    // UIの取得
    virtual PlayerUI* player_ui() = 0;
};

#endif // !IWORLD_H_
