# -*- encoding: utf-8 -*-
# stub: feedjira 4.0.2 ruby lib

Gem::Specification.new do |s|
  s.name = "feedjira".freeze
  s.version = "4.0.2".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "changelog_uri" => "https://github.com/feedjira/feedjira/blob/main/CHANGELOG.md", "homepage_uri" => "https://github.com/feedjira/feedjira", "rubygems_mfa_required" => "true", "source_code_uri" => "https://github.com/feedjira/feedjira" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["Adam Hess".freeze, "Akinori Musha".freeze, "Ezekiel Templin".freeze, "Jon Allured".freeze, "Julien Kirch".freeze, "Michael Stock".freeze, "Paul Dix".freeze]
  s.date = "1980-01-02"
  s.homepage = "https://github.com/feedjira/feedjira".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 3.1".freeze)
  s.rubygems_version = "4.0.10".freeze
  s.summary = "A feed parsing library".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<logger>.freeze, [">= 1.0".freeze, "< 2".freeze])
  s.add_runtime_dependency(%q<loofah>.freeze, [">= 2.3.1".freeze, "< 3".freeze])
  s.add_runtime_dependency(%q<sax-machine>.freeze, [">= 1.0".freeze, "< 2".freeze])
end
